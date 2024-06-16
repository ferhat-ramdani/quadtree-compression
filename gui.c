#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <float.h>
#include "gui.h"
#include "c_tree.h"
#include "bw_tree.h"
#include "color.h"
#include "binary_converter.h"
#include "minimizer.h"
#include <string.h>

void initialize_window() {
  MLV_create_window("Quadtree Compression", "Quadtree Compression", WIDTH, HEIGHT);
}

void draw_buttons() {
  DRAW_BUTTON(0, "Load Image");
  DRAW_BUTTON(1, "Quadtree Approximation");
  DRAW_BUTTON(2, "Save Binary BW");
  DRAW_BUTTON(3, "Save Binary RGBA");
  DRAW_BUTTON(4, "Minimize Quadtree");
  DRAW_BUTTON(5, "Save Minimized BW");
  DRAW_BUTTON(6, "Save Minimized RGBA");
  DRAW_BUTTON(7, "Load minimized Image");
  DRAW_BUTTON(8, "Quit");
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

void draw_shape(char shape, MLV_Color color, int x, int y, int length) {
  if(shape == '*') {
    int random = rand() % 4;
    switch(random) {
      case 0:
        shape = 'r';
        break;
      case 1:
        shape = 'c';
        break;
      case 2:
        shape = 'x';
        break;
      case 3:
        shape = '+';
        break;
    }
  }
  switch(shape) {
    case 'r':
      MLV_draw_rectangle(x, y, length, length, color);
      break;
    case 'c':
      MLV_draw_circle(x + length/2, y + length/2, length/2, color);
      break;
    case '-':
      MLV_draw_line(x, y + length/2, x + length, y + length/2, color);
      break;
    case '|':
      MLV_draw_line(x + length/2, y, x + length/2, y + length, color);
      break;
    case 'x':
      MLV_draw_line(x, y, x + length, y + length, color);
      MLV_draw_line(x, y + length, x + length, y, color);
      break;
    case '+':
      MLV_draw_line(x + length/2, y, x + length/2, y + length, color);
      MLV_draw_line(x, y + length/2, x + length, y + length/2, color);
      break;
    default:
      MLV_draw_filled_rectangle(x, y, length, length, color);
      break;
  }
} 

void draw_c_tree_as_image_aux(c_node *c_tree, int x, int y, int length, char shape) {
  if (c_tree->children == NULL) {
    MLV_Color color = MLV_rgba(c_tree->color->red, c_tree->color->green, c_tree->color->blue, c_tree->color->alpha);
    draw_shape(shape, color, x, y, length);
  } else {
    draw_c_tree_as_image_aux(c_tree->children[0], x, y, length/2, shape);
    draw_c_tree_as_image_aux(c_tree->children[1], x + length/2, y, length/2, shape);
    draw_c_tree_as_image_aux(c_tree->children[2], x + length/2, y + length/2, length/2, shape);
    draw_c_tree_as_image_aux(c_tree->children[3], x, y + length/2, length/2, shape);
  }
}

void draw_c_tree_as_image(c_node *c_tree, char shape) {
  MLV_draw_filled_rectangle(X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, IMAGE_SIZE, MLV_COLOR_BLACK);
  draw_c_tree_as_image_aux(c_tree, X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, shape);
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
float step_approximate(c_node *c_tree, MLV_Image *image, int delay, char shape) {
  float min_err = FLT_MAX;
  float max_err = FLT_MIN;
  get_min_max_err(c_tree, image, 0, 0, MLV_get_image_width(image), &min_err, &max_err);
  float ref_err = (min_err + max_err) / 3;
  split_leaves_by_err(c_tree, image, 0, 0, MLV_get_image_width(image), ref_err);
  draw_c_tree_as_image(c_tree, shape);
  MLV_wait_milliseconds(delay);
  return ref_err;
}

c_node *approximate_image(MLV_Image *image, char shape) {
  color *c = average_color(image, 1, 1, MLV_get_image_width(image), MLV_get_image_height(image));
  c_node *c_tree = create_c_leaf(c);
  int delay = 10;
  float err = FLT_MAX;
  int step = 0;
  while(err > 0.00001) {
    err = step_approximate(c_tree, image, delay, shape);
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

void draw_bw_tree_as_image_aux(bw_node *bw_tree, int x, int y, int length, char shape) {
  if (bw_tree->children == NULL) {
    MLV_Color color = bw_tree->data == 0 ? MLV_COLOR_BLACK : MLV_COLOR_WHITE;
    draw_shape(shape, color, x, y, length);
  } else {
    draw_bw_tree_as_image_aux(bw_tree->children[0], x, y, length/2, shape);
    draw_bw_tree_as_image_aux(bw_tree->children[1], x + length/2, y, length/2, shape);
    draw_bw_tree_as_image_aux(bw_tree->children[2], x + length/2, y + length/2, length/2, shape);
    draw_bw_tree_as_image_aux(bw_tree->children[3], x, y + length/2, length/2, shape);
  }
}

void draw_bw_tree_as_image(bw_node *bw_tree, char shape) {
  MLV_draw_filled_rectangle(X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, IMAGE_SIZE, MLV_COLOR_BLACK);
  draw_bw_tree_as_image_aux(bw_tree, X_ORIGIN, Y_ORIGIN, IMAGE_SIZE, shape);
  MLV_update_window();
}

int ends_with(const char *str, const char *suffix) {
  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);
  return str_len >= suffix_len &&
         strcmp(str + str_len - suffix_len, suffix) == 0;
}

void handle_buttons(MLV_Image **image, c_node **tree) {
  int x, y;
  char *filename = NULL;
  HashTable *hash_table = create_hash_table();

  while (1) {
    MLV_wait_mouse(&x, &y);

    if (x >= MARGIN_LEFT && x <= MARGIN_LEFT + BUTTON_WIDTH) {
      CLEAR_NOTIFICATION();
      if (y >= BUTTON_Y_POS(0) && y <= BUTTON_Y_POS(0) + BUTTON_HEIGHT) {
        // Load Image
        if (*image != NULL)
          MLV_free_image(*image);
        DRAW_INPUT_BOX(0, "Enter image filename: ", &filename);
        *image = MLV_load_image(filename);
        if (*image != NULL) {
          MLV_resize_image_with_proportions(*image, IMAGE_SIZE, IMAGE_SIZE);
          MLV_draw_image(*image, X_ORIGIN, Y_ORIGIN);
        } else {
          draw_text("Image not found", MLV_COLOR_RED);
        }
      } else if (y >= BUTTON_Y_POS(1) && y <= BUTTON_Y_POS(1) + BUTTON_HEIGHT) {
        // Quadtree Approximation
        if (*image != NULL) {
          *tree = approximate_image(*image, 'r');
        }
      } else if (y >= BUTTON_Y_POS(2) && y <= BUTTON_Y_POS(2) + BUTTON_HEIGHT) {
        // Save Binary BW
        if (*tree != NULL) {
          bw_node *bw_tree = convert_c_tree_to_bw_tree(*tree);
          DRAW_INPUT_BOX(2, "Enter filename to save (BW): ", &filename);
          if(!ends_with(filename, ".qtn")) {
            strcat(filename, ".qtn");
          }
          char filepath[256];
          sprintf(filepath, "compressed/%s", filename);
          save_bw_tree_binary(filepath, bw_tree);
          draw_text("Quadtree saved (BW)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(3) && y <= BUTTON_Y_POS(3) + BUTTON_HEIGHT) {
        // Save Binary RGBA
        if (*tree != NULL) {
          DRAW_INPUT_BOX(3, "Enter filename to save (RGBA): ", &filename);
          if(!ends_with(filename, ".qtc")) {
            strcat(filename, ".qtc");
          }
          char filepath[256];
          sprintf(filepath, "compressed/%s", filename);
          save_c_tree_binary(filepath, *tree);
          draw_text("Quadtree saved (RGBA)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(4) && y <= BUTTON_Y_POS(4) + BUTTON_HEIGHT) {
        // Minimize Quadtree
        if (*tree != NULL) {
          minimize_identical_leaves_in_node(*tree);
          *tree = minimize_unique_leaves_aux(*tree, hash_table);
          draw_c_tree_as_image(*tree, '.');
          draw_text("Quadtree minimized", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(5) && y <= BUTTON_Y_POS(5) + BUTTON_HEIGHT) {
        // Save Minimized BW
        if (*tree != NULL) {
          DRAW_INPUT_BOX(5, "Enter filename to save (Minimized BW): ", &filename);
          if(!ends_with(filename, ".qtn")) {
            strcat(filename, ".qtn");
          }
          bw_node *bw_tree = convert_c_tree_to_bw_tree(*tree);
          save_bw_tree_binary(filename, bw_tree);
          draw_text("Quadtree saved (Minimized BW)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(6) && y <= BUTTON_Y_POS(6) + BUTTON_HEIGHT) {
        // Save Minimized RGBA
        if (*tree != NULL) {
          DRAW_INPUT_BOX(6, "Enter filename to save (Minimized RGBA): ", &filename);
          if(!ends_with(filename, ".qtc")) {
            strcat(filename, ".qtc");
          }
          save_c_tree_binary(filename, *tree);
          draw_text("Quadtree saved (Minimized RGBA)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(7) && y <= BUTTON_Y_POS(7) + BUTTON_HEIGHT) {
        // Open Image
        DRAW_INPUT_BOX(7, "Enter filename to open: ", &filename);
        if(ends_with(filename, ".qtc")) {
          *tree = load_c_tree_binary(filename);
          if (*tree != NULL) {
            draw_c_tree_as_image(*tree, '.');
            draw_text("Minimizd colored quadtree loaded", MLV_COLOR_YELLOW);
          } else {
            draw_text("Internal error", MLV_COLOR_RED);
          }
        }
        else if(ends_with(filename, ".qtn")) {
          bw_node *bw_tree = load_bw_tree_binary(filename);
          if (bw_tree != NULL) {
            draw_bw_tree_as_image(bw_tree, '.');
            draw_text("Minimizd BW quadtree loaded", MLV_COLOR_YELLOW);
          } else {
            draw_text("Internal error", MLV_COLOR_RED);
          }
        } else {
          draw_text("Wrong format", MLV_COLOR_RED);
        }
      } else if (y >= BUTTON_Y_POS(8) && y <= BUTTON_Y_POS(8) + BUTTON_HEIGHT) {
        // Quit
        break;
      }
      MLV_update_window();
    }
  }
  free_hash_table(hash_table);
}
