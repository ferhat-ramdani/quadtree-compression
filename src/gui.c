#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include "gui.h"
#include "c_tree.h"
#include "bw_tree.h"
#include "color.h"
#include "binary_converter.h"
#include "minimizer.h"
#include "utils.h"
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

void initialize_window() {
  MLV_create_window("Quadtree Compression", "Quadtree Compression", WIDTH, HEIGHT);
}

void draw_shape(char shape, bool is_filled, MLV_Color color, int x, int y, int length);

void draw_buttons(bool is_filled, char shape) {
  DRAW_BUTTON(0, "Load Image");
  DRAW_BUTTON(1, "Approximate Quadtree");
  DRAW_BUTTON(2, "Minimize Quadtree");
  DRAW_BUTTON(3, "Save Image");

  char mode_text[50];
  sprintf(mode_text, "Toggle Mode: %s", is_filled ? "Filled" : "Outline");
  DRAW_BUTTON(4, mode_text);

  char shape_text[50];
  char* shape_name;
  switch(shape) {
      case 't': shape_name = "Triangle"; break;
      case 'c': shape_name = "Circle"; break;
      case 'r': shape_name = "Cube"; break;
      case 's': shape_name = "Star"; break;
      case '*': shape_name = "Mix"; break;
      case 'x': shape_name = "X Shape"; break;
      default: shape_name = "Unknown";
  }
  sprintf(shape_text, "Toggle Shape: %s", shape_name);
  DRAW_BUTTON(5, shape_text);
  
  // Draw the shape at the right edge of button 5
  int draw_length = BUTTON_HEIGHT - 20;
  int draw_x = MARGIN_LEFT + BUTTON_WIDTH - draw_length - 15;
  int draw_y = BUTTON_Y_POS(5) + 10;
  if (shape == '*') {
     int sub_len = draw_length / 2;
     draw_shape('t', is_filled, MLV_COLOR_WHITE, draw_x, draw_y, sub_len);
     draw_shape('c', is_filled, MLV_COLOR_WHITE, draw_x + sub_len, draw_y, sub_len);
     draw_shape('r', is_filled, MLV_COLOR_WHITE, draw_x, draw_y + sub_len, sub_len);
     draw_shape('s', is_filled, MLV_COLOR_WHITE, draw_x + sub_len, draw_y + sub_len, sub_len);
  } else {
     draw_shape(shape, is_filled, MLV_COLOR_WHITE, draw_x, draw_y, draw_length);
  }

  DRAW_BUTTON_RED(6, "Quit Application");
  
  MLV_update_window();
}

void draw_text(char *text, MLV_Color color) {
  CLEAR_NOTIFICATION();
  int width, height;
  MLV_get_size_of_text(text, &width, &height);
  MLV_draw_text(MARGIN_LEFT + BUTTON_WIDTH + (WIDTH - MARGIN_LEFT - BUTTON_WIDTH) / 2 - width/2, HEIGHT/20 - height/2, text, color);
}

color *average_color(MLV_Image *image, int x, int y, int width, int height) {
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

c_node *create_c_tree_from_image(MLV_Image *image, int x, int y, int length) {
  if (length <= 1) {
    color *c = average_color(image, x, y, length, length);
    return create_c_leaf(c);
  }

  c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
  children[0] = create_c_tree_from_image(image, x, y, length/2);
  children[1] = create_c_tree_from_image(image, x + length/2, y, length/2);
  children[2] = create_c_tree_from_image(image, x + length/2, y + length/2, length/2);
  children[3] = create_c_tree_from_image(image, x, y + length/2, length/2);

  return create_c_node(children);
}

void draw_shape(char shape, bool is_filled, MLV_Color color, int x, int y, int length) {
  if (shape == '*') {
     char shapes_filled[] = {'t', 'c', 'r', 's'};
     char shapes_outline[] = {'t', 'c', 'r', 's', 'x'};
     if (is_filled) {
         shape = shapes_filled[rand() % 4];
     } else {
         shape = shapes_outline[rand() % 5];
     }
  }

  if (is_filled) {
    switch (shape) {
      case 't': {
        int vx[3] = {x + length/2, x, x + length};
        int vy[3] = {y, y + length, y + length};
        MLV_draw_filled_polygon(vx, vy, 3, color);
        break;
      }
      case 'c':
        MLV_draw_filled_circle(x + length/2, y + length/2, length/2, color);
        break;
      case 's': {
        int vx[10], vy[10];
        int cx = x + length/2, cy = y + length/2;
        float R = length/2.0;
        float r = length/4.0;
        for (int i = 0; i < 10; i++) {
            float angle = i * 3.14159265 / 5.0 - 3.14159265 / 2.0;
            float rad = (i % 2 == 0) ? R : r;
            vx[i] = cx + (int)(rad * cos(angle));
            vy[i] = cy + (int)(rad * sin(angle));
        }
        MLV_draw_filled_polygon(vx, vy, 10, color);
        break;
      }
      case 'r':
      default:
        MLV_draw_filled_rectangle(x, y, length, length, color);
        break;
    }
  } else {
    switch(shape) {
      case 't': {
        int vx[3] = {x + length/2, x, x + length};
        int vy[3] = {y, y + length, y + length};
        MLV_draw_polygon(vx, vy, 3, color);
        break;
      }
      case 'c':
        MLV_draw_circle(x + length/2, y + length/2, length/2, color);
        break;
      case 's': {
        int vx[10], vy[10];
        int cx = x + length/2, cy = y + length/2;
        float R = length/2.0;
        float r = length/4.0;
        for (int i = 0; i < 10; i++) {
            float angle = i * 3.14159265 / 5.0 - 3.14159265 / 2.0;
            float rad = (i % 2 == 0) ? R : r;
            vx[i] = cx + (int)(rad * cos(angle));
            vy[i] = cy + (int)(rad * sin(angle));
        }
        MLV_draw_polygon(vx, vy, 10, color);
        break;
      }
      case 'x':
        MLV_draw_line(x, y, x + length, y + length, color);
        MLV_draw_line(x, y + length, x + length, y, color);
        break;
      case 'r':
      default:
        MLV_draw_rectangle(x, y, length, length, color);
        break;
    }
  }
} 

void draw_c_tree_as_image_aux(c_node *c_tree, int x, int y, int length, char shape, bool is_filled) {
  if (c_tree->children == NULL) {
    MLV_Color color = MLV_rgba(c_tree->color->red, c_tree->color->green, c_tree->color->blue, c_tree->color->alpha);
    draw_shape(shape, is_filled, color, x, y, length);
  } else {
    draw_c_tree_as_image_aux(c_tree->children[0], x, y, length/2, shape, is_filled);
    draw_c_tree_as_image_aux(c_tree->children[1], x + length/2, y, length/2, shape, is_filled);
    draw_c_tree_as_image_aux(c_tree->children[2], x + length/2, y + length/2, length/2, shape, is_filled);
    draw_c_tree_as_image_aux(c_tree->children[3], x, y + length/2, length/2, shape, is_filled);
  }
}

void draw_c_tree_as_image(c_node *c_tree, char shape, bool is_filled) {
  MLV_draw_filled_rectangle(X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, IMAGE_SIZE, MLV_COLOR_BLACK);
  draw_c_tree_as_image_aux(c_tree, X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, shape, is_filled);
  MLV_update_window();
}

/* Computes the error of a leaf */
float c_leaf_err(c_node *c_tree, MLV_Image *image, int x, int y, int length) {
  float err = 0.0;
  int i, j;
  for(i = 0; i < length; i++) {
    for(j = 0; j < length; j++) {
      int vr, vg, vb, va;
      MLV_get_pixel_on_image(image, x + i, y + j, &vr, &vg, &vb, &va);
      color *c = create_color(vr, vg, vb, va);
      err += c_c_distance(*c, *(c_tree->color));
      free_color(c);
    }
  }
  return err;
}

void get_min_max_err(c_node *c_tree, MLV_Image *image, int x, int y, int length, float *min_err, float *max_err) {
  if(c_tree == NULL) return;
  if (c_tree->children == NULL) {
    float err = c_leaf_err(c_tree, image, x, y, length);
    if (err < *min_err) {
      *min_err = err;
    }
    if (err > *max_err) {
      *max_err = err;
    }
  } else {
    get_min_max_err(c_tree->children[0], image, x, y, length/2, min_err, max_err);
    get_min_max_err(c_tree->children[1], image, x + length/2, y, length/2, min_err, max_err);
    get_min_max_err(c_tree->children[2], image, x + length/2, y + length/2, length/2, min_err, max_err);
    get_min_max_err(c_tree->children[3], image, x, y + length/2, length/2, min_err, max_err);
  }
}

void split_leaves_by_err(c_node *c_tree, MLV_Image *image, int x, int y, int length, float ref_err) {
  if(c_tree == NULL) return;
  if (c_tree->children == NULL) {
    float err = c_leaf_err(c_tree, image, x, y, length);
    if(err >= ref_err) {
      // split the node
      c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
      color *c1 = average_color(image, x, y, length/2, length/2);
      color *c2 = average_color(image, x + length/2, y, length/2, length/2);
      color *c3 = average_color(image, x + length/2, y + length/2, length/2, length/2);
      color *c4 = average_color(image, x, y + length/2, length/2, length/2);
      children[0] = create_c_leaf(c1);
      children[1] = create_c_leaf(c2);
      children[2] = create_c_leaf(c3);
      children[3] = create_c_leaf(c4);
      c_tree->children = children;
    }
  } else {
    split_leaves_by_err(c_tree->children[0], image, x, y, length/2, ref_err);
    split_leaves_by_err(c_tree->children[1], image, x + length/2, y, length/2, ref_err);
    split_leaves_by_err(c_tree->children[2], image, x + length/2, y + length/2, length/2, ref_err);
    split_leaves_by_err(c_tree->children[3], image, x, y + length/2, length/2, ref_err);
  }
}

/*
 * Approximates the image by one step, by splitting the nodes which errors 
 * are below a certain reference error = (min_err + max_err) / 3
 * where min_err and max_err are the minimum and maximum errors of the leaves
 */
float step_approximate(c_node *c_tree, MLV_Image *image, int delay, char shape, bool is_filled) {
  float min_err = FLT_MAX;
  float max_err = FLT_MIN;
  get_min_max_err(c_tree, image, 0, 0, MLV_get_image_width(image), &min_err, &max_err);
  float ref_err = (min_err + max_err) / 3;
  split_leaves_by_err(c_tree, image, 0, 0, MLV_get_image_width(image), ref_err);
  draw_c_tree_as_image(c_tree, shape, is_filled);
  MLV_wait_milliseconds(delay);
  return ref_err;
}

c_node *approximate_image(MLV_Image *image, char shape, bool is_filled) {
  color *c = average_color(image, 1, 1, MLV_get_image_width(image), MLV_get_image_height(image));
  c_node *c_tree = create_c_leaf(c);
  int delay = 10;
  float err = FLT_MAX;
  int step = 0;
  while(err > 0.00001) {
    err = step_approximate(c_tree, image, delay, shape, is_filled);
    printf("Step %d, err %.2f\n", step, err);
    step++;
  }
  return c_tree;
}

unsigned char from_color_to_bw(color c) {
  int sum = c.red + c.green + c.blue + c.alpha;
  return (unsigned char) (sum > ((255 / 2) * 4) ? 1 : 0);
}

bw_node *convert_c_tree_to_bw_tree(c_node *c_tree) {
  if (c_tree->children == NULL) {
    return create_bw_leaf(from_color_to_bw(*(c_tree->color)));
  } else {
    bw_node **children = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
    children[0] = convert_c_tree_to_bw_tree(c_tree->children[0]);
    children[1] = convert_c_tree_to_bw_tree(c_tree->children[1]);
    children[2] = convert_c_tree_to_bw_tree(c_tree->children[2]);
    children[3] = convert_c_tree_to_bw_tree(c_tree->children[3]);
    return create_bw_node(1, children);
  }
}

void draw_bw_tree_as_image_aux(bw_node *bw_tree, int x, int y, int length, char shape, bool is_filled) {
  if (bw_tree->children == NULL) {
    MLV_Color color = bw_tree->data == 0 ? MLV_COLOR_BLACK : MLV_COLOR_WHITE;
    draw_shape(shape, is_filled, color, x, y, length);
  } else {
    draw_bw_tree_as_image_aux(bw_tree->children[0], x, y, length/2, shape, is_filled);
    draw_bw_tree_as_image_aux(bw_tree->children[1], x + length/2, y, length/2, shape, is_filled);
    draw_bw_tree_as_image_aux(bw_tree->children[2], x + length/2, y + length/2, length/2, shape, is_filled);
    draw_bw_tree_as_image_aux(bw_tree->children[3], x, y + length/2, length/2, shape, is_filled);
  }
}

void draw_bw_tree_as_image(bw_node *bw_tree, char shape, bool is_filled) {
  MLV_draw_filled_rectangle(X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, IMAGE_SIZE, MLV_COLOR_BLACK);
  draw_bw_tree_as_image_aux(bw_tree, X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, shape, is_filled);
  MLV_update_window();
}

int ends_with(const char *str, const char *suffix) {
  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);
  return str_len >= suffix_len &&
         strcmp(str + str_len - suffix_len, suffix) == 0;
}

void create_folder(char *filename){
  DIR* dir = opendir(filename);
  if (dir){
    closedir(dir);
  }else if(ENOENT == errno){
#ifdef _WIN32
    mkdir(filename);
#else
    mkdir(filename, 0755);
#endif
  }
}

void handle_buttons(MLV_Image **image, c_node **tree) {
  int x, y;
  char *filename = NULL;
  HashTable *hash_table = create_hash_table();
  create_folder("compressed");

  bool is_filled = true;
  char current_shape = 'r';
  bool is_bw_tree = false;
  bw_node *bw_tree = NULL;

  while (1) {
    MLV_wait_mouse(&x, &y);

    if (x >= MARGIN_LEFT && x <= MARGIN_LEFT + BUTTON_WIDTH) {
      CLEAR_NOTIFICATION();
      if (y >= BUTTON_Y_POS(0) && y <= BUTTON_Y_POS(0) + BUTTON_HEIGHT) {
        // Load Image
        filename = open_file_dialog("../imgs");
        if (filename != NULL) {
          if (ends_with(filename, ".qtc")) {
            if (*tree != NULL) free_c_tree(*tree);
            *tree = load_c_tree_binary(filename);
            if (*tree != NULL) {
              is_bw_tree = false;
              draw_c_tree_as_image(*tree, current_shape, is_filled);
              draw_text("Colored quadtree loaded", MLV_COLOR_YELLOW);
            } else {
              draw_text("Internal error", MLV_COLOR_RED);
            }
          } else if (ends_with(filename, ".qtn")) {
            if (bw_tree != NULL) free_bw_tree(bw_tree);
            bw_tree = load_bw_tree_binary(filename);
            if (bw_tree != NULL) {
              is_bw_tree = true;
              draw_bw_tree_as_image(bw_tree, current_shape, is_filled);
              draw_text("BW quadtree loaded", MLV_COLOR_YELLOW);
            } else {
              draw_text("Internal error", MLV_COLOR_RED);
            }
          } else {
            if (*image != NULL) MLV_free_image(*image);
            *image = MLV_load_image(filename);
            if (*image != NULL) {
              MLV_resize_image_with_proportions(*image, IMAGE_SIZE, IMAGE_SIZE);
              MLV_draw_image(*image, X_ORIGIN, Y_ORIGIN);
            } else {
              draw_text("Image not found", MLV_COLOR_RED);
            }
          }
          free(filename);
        }
      } else if (y >= BUTTON_Y_POS(1) && y <= BUTTON_Y_POS(1) + BUTTON_HEIGHT) {
        // Approximate Quadtree
        if (*image != NULL) {
          if (*tree != NULL) free_c_tree(*tree);
          free_hash_table(hash_table);
          hash_table = create_hash_table();
          *tree = approximate_image(*image, current_shape, is_filled);
          is_bw_tree = false;
        }
      } else if (y >= BUTTON_Y_POS(2) && y <= BUTTON_Y_POS(2) + BUTTON_HEIGHT) {
        // Minimize Quadtree
        if (*image != NULL) {
          if (*tree != NULL) free_c_tree(*tree);
          free_hash_table(hash_table);
          hash_table = create_hash_table();
          *tree = approximate_image(*image, current_shape, is_filled);
          minimize_identical_leaves_in_node(*tree);
          *tree = minimize_unique_leaves(*tree, hash_table);
          is_bw_tree = false;
          draw_c_tree_as_image(*tree, current_shape, is_filled);
          draw_text("Quadtree minimized", MLV_COLOR_YELLOW);
        } else if (is_bw_tree && bw_tree != NULL) {
          draw_text("Cannot minimize BW tree", MLV_COLOR_RED);
        } else if (!is_bw_tree && *tree != NULL) {
          // Fallback if we only have a loaded tree and no original image
          minimize_identical_leaves_in_node(*tree);
          *tree = minimize_unique_leaves(*tree, hash_table);
          draw_c_tree_as_image(*tree, current_shape, is_filled);
          draw_text("Quadtree minimized", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(3) && y <= BUTTON_Y_POS(3) + BUTTON_HEIGHT) {
        // Save Image
        if (is_bw_tree && bw_tree != NULL) {
          filename = save_file_dialog("../compressed");
          if (filename != NULL) {
            if(!ends_with(filename, ".qtn")) {
              char* new_filename = malloc(strlen(filename) + 5);
              strcpy(new_filename, filename);
              strcat(new_filename, ".qtn");
              free(filename);
              filename = new_filename;
            }
            save_bw_tree_binary(filename, bw_tree);
            draw_text("Quadtree saved (BW)", MLV_COLOR_YELLOW);
            free(filename);
          }
        } else if (!is_bw_tree && *tree != NULL) {
          filename = save_file_dialog("../compressed");
          if (filename != NULL) {
            if (ends_with(filename, ".qtn")) {
              bw_node* new_bw = convert_c_tree_to_bw_tree(*tree);
              save_bw_tree_binary(filename, new_bw);
              free_bw_tree(new_bw);
              draw_text("Quadtree saved (BW)", MLV_COLOR_YELLOW);
            } else {
              if(!ends_with(filename, ".qtc")) {
                char* new_filename = malloc(strlen(filename) + 5);
                strcpy(new_filename, filename);
                strcat(new_filename, ".qtc");
                free(filename);
                filename = new_filename;
              }
              save_c_tree_binary(filename, *tree);
              draw_text("Quadtree saved (RGBA)", MLV_COLOR_YELLOW);
            }
            free(filename);
          }
        }
      } else if (y >= BUTTON_Y_POS(4) && y <= BUTTON_Y_POS(4) + BUTTON_HEIGHT) {
        // Toggle Mode
        is_filled = !is_filled;
        if (is_filled && current_shape == 'x') {
            current_shape = 't'; // x shape is not supported in filled mode
        }
        draw_buttons(is_filled, current_shape);
        if (is_bw_tree && bw_tree != NULL) {
            draw_bw_tree_as_image(bw_tree, current_shape, is_filled);
        } else if (!is_bw_tree && *tree != NULL) {
            draw_c_tree_as_image(*tree, current_shape, is_filled);
        }
      } else if (y >= BUTTON_Y_POS(5) && y <= BUTTON_Y_POS(5) + BUTTON_HEIGHT) {
        // Toggle Shape
        char shapes_filled[] = {'t', 'c', 'r', 's', '*'};
        char shapes_outline[] = {'t', 'c', 'r', 's', '*', 'x'};
        char* shapes = is_filled ? shapes_filled : shapes_outline;
        int num_shapes = is_filled ? 5 : 6;
        int i;
        for(i = 0; i < num_shapes; i++) {
            if (shapes[i] == current_shape) break;
        }
        current_shape = shapes[(i + 1) % num_shapes];
        draw_buttons(is_filled, current_shape);
        if (is_bw_tree && bw_tree != NULL) {
            draw_bw_tree_as_image(bw_tree, current_shape, is_filled);
        } else if (!is_bw_tree && *tree != NULL) {
            draw_c_tree_as_image(*tree, current_shape, is_filled);
        }
      } else if (y >= BUTTON_Y_POS(6) && y <= BUTTON_Y_POS(6) + BUTTON_HEIGHT) {
        // Quit Application
        break;
      }
      MLV_update_window();
    }
  }
  free_hash_table(hash_table);
}
