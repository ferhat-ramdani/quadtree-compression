#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <float.h>
#include "gui.h"
#include "c_tree.h"
#include "color.h"
#include "binary_converter.h"
#include "minimizer.h"

#define IMAGE_SIZE 512
#define NB_BUTTONS 9
#define WIDTH IMAGE_SIZE * 2
#define HEIGHT IMAGE_SIZE * 3/2
#define BUTTON_WIDTH WIDTH / 4
#define BUTTON_HEIGHT HEIGHT / 15
#define MARGIN_LEFT WIDTH / 20
#define BUTTON_MARGIN HEIGHT / 30
#define MARGIN_TOP (HEIGHT - (BUTTON_HEIGHT * NB_BUTTONS + (NB_BUTTONS - 1) * BUTTON_MARGIN)) / 2
#define X_ORIGIN MARGIN_LEFT + BUTTON_WIDTH + (WIDTH - MARGIN_LEFT - BUTTON_WIDTH) / 2 - 256
#define Y_ORIGIN HEIGHT/2 - IMAGE_SIZE/2
#define INPUT_BOX_WIDTH WIDTH / 4
#define INPUT_BOX_HEIGHT BUTTON_HEIGHT
#define INPUT_BOX_X MARGIN_LEFT + BUTTON_WIDTH + 10
#define INPUT_BOX_Y(y) (BUTTON_Y_POS(y))
#define DRAW_INPUT_BOX(y, prompt, filename) \
  MLV_wait_input_box(INPUT_BOX_X, INPUT_BOX_Y(y), INPUT_BOX_X + INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, prompt, filename);
#define BUTTON_Y_POS(y) (MARGIN_TOP + BUTTON_HEIGHT * (y) + BUTTON_MARGIN * (y))
#define DRAW_BUTTON(y, text) \
  MLV_draw_text_box(MARGIN_LEFT, BUTTON_Y_POS(y), BUTTON_WIDTH, BUTTON_HEIGHT, text, 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER)


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
  DRAW_BUTTON(7, "Load minimized image");
  DRAW_BUTTON(8, "Quit");
  MLV_update_window();
}

void draw_text(char *text, MLV_Color color) {
  MLV_draw_filled_rectangle(MARGIN_LEFT + BUTTON_WIDTH, 0, WIDTH, HEIGHT/10, MLV_COLOR_BLACK);
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
  color *c = average_color(image, 0, 0, MLV_get_image_width(image), MLV_get_image_height(image));
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

void handle_buttons(MLV_Image **image, c_node **tree) {
  int x, y;
  char *filename = NULL;

  while (1) {
    MLV_wait_mouse(&x, &y);

    if (x >= MARGIN_LEFT && x <= MARGIN_LEFT + BUTTON_WIDTH) {
      if (y >= BUTTON_Y_POS(0) && y <= BUTTON_Y_POS(0) + BUTTON_HEIGHT) {
        // Load Image
        if (*image != NULL)
          MLV_free_image(*image);
        DRAW_INPUT_BOX(0, "Enter image filename: ", &filename);
        *image = MLV_load_image(filename);
        if (*image != NULL) {
          MLV_resize_image_with_proportions(*image, IMAGE_SIZE, IMAGE_SIZE);
          MLV_draw_image(*image, X_ORIGIN, Y_ORIGIN);
        }
      } else if (y >= BUTTON_Y_POS(1) && y <= BUTTON_Y_POS(1) + BUTTON_HEIGHT) {
        // Quadtree Approximation
        if (*image != NULL) {
          *tree = approximate_image(*image, '*');
        }
      } else if (y >= BUTTON_Y_POS(2) && y <= BUTTON_Y_POS(2) + BUTTON_HEIGHT) {
        // Save Binary BW
        if (*tree != NULL) {
          DRAW_INPUT_BOX(2, "Enter filename to save (BW): ", &filename);
          save_c_tree_binary(filename, *tree);
          draw_text("Quadtree saved (BW)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(3) && y <= BUTTON_Y_POS(3) + BUTTON_HEIGHT) {
        // Save Binary RGBA
        if (*tree != NULL) {
          DRAW_INPUT_BOX(3, "Enter filename to save (RGBA): ", &filename);
          save_c_tree_binary(filename, *tree);
          draw_text("Quadtree saved (RGBA)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(4) && y <= BUTTON_Y_POS(4) + BUTTON_HEIGHT) {
        // Minimize Quadtree
        if (*tree != NULL) {
          minimize_identical_leaves_in_node(*tree);
          draw_text("Quadtree minimized", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(5) && y <= BUTTON_Y_POS(5) + BUTTON_HEIGHT) {
        // Save Minimized BW
        if (*tree != NULL) {
          DRAW_INPUT_BOX(5, "Enter filename to save (Minimized BW): ", &filename);
          save_c_tree_binary(filename, *tree);
          draw_text("Quadtree saved (Minimized BW)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(6) && y <= BUTTON_Y_POS(6) + BUTTON_HEIGHT) {
        // Save Minimized RGBA
        if (*tree != NULL) {
          DRAW_INPUT_BOX(6, "Enter filename to save (Minimized RGBA): ", &filename);
          save_c_tree_binary(filename, *tree);
          draw_text("Quadtree saved (Minimized RGBA)", MLV_COLOR_YELLOW);
        }
      } else if (y >= BUTTON_Y_POS(7) && y <= BUTTON_Y_POS(7) + BUTTON_HEIGHT) {
        // Open Image
        DRAW_INPUT_BOX(7, "Enter filename to open: ", &filename);
        *tree = load_c_tree_binary(filename);
        if (*tree != NULL) {
          draw_text("Quadtree loaded", MLV_COLOR_YELLOW);
          draw_c_tree_as_image(*tree, '*');
        }
      }
      MLV_update_window();
    }
  }
}

