#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <MLV/MLV_all.h>

#include "c_tree.h"
#include "bw_tree.h"
#include "color.h"
#include "binary_converter.h"
#include "minimizer.h"
#include "utils.h"
#include <sys/stat.h>

// Helper from gui.c to calculate average color
color *average_color_bm(MLV_Image *image, int x, int y, int width, int height) {
  int r = 0, g = 0, b = 0, a = 0, count = 0;
  for (int i = x; i < x + width; i++) {
    for (int j = y; j < y + height; j++) {
      int vr, vg, vb, va;
      MLV_get_pixel_on_image(image, i, j, &vr, &vg, &vb, &va);
      r += vr;
      g += vg;
      b += vb;
      a += va;
      count++;
    }
  }
  return create_color(r / count, g / count, b / count, a / count);
}

// Helper from gui.c
float c_leaf_err_bm(c_node *c_tree, MLV_Image *image, int x, int y, int length) {
  float err = 0.0;
  for(int i = 0; i < length; i++) {
    for(int j = 0; j < length; j++) {
      int vr, vg, vb, va;
      MLV_get_pixel_on_image(image, x + i, y + j, &vr, &vg, &vb, &va);
      color *c = create_color(vr, vg, vb, va);
      err += c_c_distance(*c, *(c_tree->color));
      free_color(c);
    }
  }
  return err;
}

void get_min_max_err_bm(c_node *c_tree, MLV_Image *image, int x, int y, int length, float *min_err, float *max_err) {
  if(c_tree == NULL) return;
  if (c_tree->children == NULL) {
    float err = c_leaf_err_bm(c_tree, image, x, y, length);
    if (err < *min_err) *min_err = err;
    if (err > *max_err) *max_err = err;
  } else {
    get_min_max_err_bm(c_tree->children[0], image, x, y, length/2, min_err, max_err);
    get_min_max_err_bm(c_tree->children[1], image, x + length/2, y, length/2, min_err, max_err);
    get_min_max_err_bm(c_tree->children[2], image, x + length/2, y + length/2, length/2, min_err, max_err);
    get_min_max_err_bm(c_tree->children[3], image, x, y + length/2, length/2, min_err, max_err);
  }
}

void split_leaves_by_err_bm(c_node *c_tree, MLV_Image *image, int x, int y, int length, float ref_err) {
  if(c_tree == NULL) return;
  if (c_tree->children == NULL) {
    float err = c_leaf_err_bm(c_tree, image, x, y, length);
    if(err >= ref_err) {
      c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
      color *c1 = average_color_bm(image, x, y, length/2, length/2);
      color *c2 = average_color_bm(image, x + length/2, y, length/2, length/2);
      color *c3 = average_color_bm(image, x + length/2, y + length/2, length/2, length/2);
      color *c4 = average_color_bm(image, x, y + length/2, length/2, length/2);
      children[0] = create_c_leaf(c1);
      children[1] = create_c_leaf(c2);
      children[2] = create_c_leaf(c3);
      children[3] = create_c_leaf(c4);
      c_tree->children = children;
    }
  } else {
    split_leaves_by_err_bm(c_tree->children[0], image, x, y, length/2, ref_err);
    split_leaves_by_err_bm(c_tree->children[1], image, x + length/2, y, length/2, ref_err);
    split_leaves_by_err_bm(c_tree->children[2], image, x + length/2, y + length/2, length/2, ref_err);
    split_leaves_by_err_bm(c_tree->children[3], image, x, y + length/2, length/2, ref_err);
  }
}

float step_approximate_bm(c_node *c_tree, MLV_Image *image) {
  float min_err = FLT_MAX;
  float max_err = FLT_MIN;
  get_min_max_err_bm(c_tree, image, 0, 0, MLV_get_image_width(image), &min_err, &max_err);
  float ref_err = (min_err + max_err) / 3;
  split_leaves_by_err_bm(c_tree, image, 0, 0, MLV_get_image_width(image), ref_err);
  return ref_err;
}

c_node *approximate_image_bm(MLV_Image *image) {
  color *c = average_color_bm(image, 1, 1, MLV_get_image_width(image), MLV_get_image_height(image));
  c_node *c_tree = create_c_leaf(c);
  float err = FLT_MAX;
  int step = 0;
  // Limit steps to avoid infinite loops if error threshold isn't reached
  while(err > 0.00001 && step < 20) {
    err = step_approximate_bm(c_tree, image);
    step++;
  }
  return c_tree;
}

unsigned char from_color_to_bw_bm(color c) {
  int sum = c.red + c.green + c.blue + c.alpha;
  return (unsigned char) (sum > ((255 / 2) * 4) ? 1 : 0);
}

bw_node *convert_c_tree_to_bw_tree_bm(c_node *c_tree) {
  if (c_tree->children == NULL) {
    return create_bw_leaf(from_color_to_bw_bm(*(c_tree->color)));
  } else {
    bw_node **children = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
    children[0] = convert_c_tree_to_bw_tree_bm(c_tree->children[0]);
    children[1] = convert_c_tree_to_bw_tree_bm(c_tree->children[1]);
    children[2] = convert_c_tree_to_bw_tree_bm(c_tree->children[2]);
    children[3] = convert_c_tree_to_bw_tree_bm(c_tree->children[3]);
    return create_bw_node(1, children);
  }
}

long get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

void process_image(const char *img_path) {
    printf("Processing: %s\n", img_path);
    MLV_Image *image = MLV_load_image(img_path);
    if (!image) {
        printf("Failed to load image.\n");
        return;
    }
    MLV_resize_image_with_proportions(image, 512, 512);
    
    // Approximate
    c_node *tree = approximate_image_bm(image);
    
    // Minimize
    HashTable *hash_table = create_hash_table();
    minimize_identical_leaves_in_node(tree);
    tree = minimize_unique_leaves(tree, hash_table);
    
    // BW tree
    bw_node *bw_tree = convert_c_tree_to_bw_tree_bm(tree);
    
    char qtc_path[256];
    char qtn_path[256];
    sprintf(qtc_path, "%s.qtc", img_path);
    sprintf(qtn_path, "%s.qtn", img_path);
    
    save_c_tree_binary(qtc_path, tree);
    save_bw_tree_binary(qtn_path, bw_tree);
    
    long orig_size = get_file_size(img_path);
    long qtc_size = get_file_size(qtc_path);
    long qtn_size = get_file_size(qtn_path);
    
    FILE *out = fopen("benchmark_results.txt", "a");
    fprintf(out, "| %s | %ld bytes | %ld bytes | %ld bytes |\n", img_path, orig_size, qtc_size, qtn_size);
    fclose(out);
    
    free_c_tree(tree);
    free_bw_tree(bw_tree);
    free_hash_table(hash_table);
    MLV_free_image(image);
}

#undef main
int main(int argc, char *argv[]) {
    freopen("stdout.log", "w", stdout);
    freopen("stderr.log", "w", stderr);
    
    MLV_create_window("Benchmark", "Benchmark", 512, 512);
    
    FILE *out = fopen("benchmark_results.txt", "a");
    fprintf(out, "| Image | Original Size | QTC Size | QTN Size |\n");
    fprintf(out, "|---|---|---|---|\n");
    fclose(out);
    
    process_image("../imgs/backpack.jpg");
    process_image("../imgs/board.jpg");
    process_image("../imgs/earth.jpeg");
    process_image("../imgs/ninja.jpg");
    process_image("../imgs/chess.png");
    process_image("../imgs/monochrome.jpg");
    
    MLV_free_window();
    return 0;
}
