#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "gui.h"
#include "c_tree.h"
#include "color.h"
#include "binary_converter.h"
#include "minimizer.h"

#define WIDTH 1000
#define HEIGHT 800
#define BUTTON_WIDTH WIDTH / 4
#define BUTTON_HEIGHT HEIGHT / 15
#define NB_BUTTONS 8
#define MARGIN_LEFT WIDTH / 20
#define BUTTON_MARGIN HEIGHT / 30
#define MARGIN_TOP (HEIGHT - (BUTTON_HEIGHT * NB_BUTTONS + (NB_BUTTONS - 1) * BUTTON_MARGIN)) / 2
#define X_ORIGIN MARGIN_LEFT + BUTTON_WIDTH + (WIDTH - MARGIN_LEFT - BUTTON_WIDTH) / 2 - 256
#define Y_ORIGIN HEIGHT/2 - 256
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
  DRAW_BUTTON(7, "Open Image");
  MLV_update_window();
}

void handle_buttons(MLV_Image **image, c_node **tree) {
  int x, y;
  char *filename = NULL;

  while (1) {
  MLV_wait_mouse(&x, &y);

  if (x >= MARGIN_LEFT && x <= MARGIN_LEFT + BUTTON_WIDTH) {
    if (y >= BUTTON_Y_POS(0) && y <= BUTTON_Y_POS(0) + BUTTON_HEIGHT) {
    // Load Image
    if (*image != NULL) {
      MLV_free_image(*image);
    }
    DRAW_INPUT_BOX(0, "Enter image filename: ", &filename);
    *image = MLV_load_image(filename);
    if (*image != NULL) {
      MLV_resize_image_with_proportions(*image, 512, 512);
      MLV_draw_image(*image, X_ORIGIN, Y_ORIGIN);
      MLV_update_window();
    }
    } else if (y >= BUTTON_Y_POS(1) && y <= BUTTON_Y_POS(1) + BUTTON_HEIGHT) {
    // Quadtree Approximation
    if (*image != NULL) {
    //   *tree = create_c_tree_from_image(*image, 0, 0, MLV_get_image_width(*image), MLV_get_image_height(*image));
    //   MLV_draw_text(200, 10, "Quadtree created", MLV_COLOR_RED);
    //   MLV_update_window();
      approximate_image(*image);
    }
    } else if (y >= BUTTON_Y_POS(2) && y <= BUTTON_Y_POS(2) + BUTTON_HEIGHT) {
    // Save Binary BW
    if (*tree != NULL) {
      DRAW_INPUT_BOX(2, "Enter filename to save (BW): ", &filename);
      save_quadtree_binary(*tree, filename);
      MLV_draw_text(200, 10, "Quadtree saved (BW)", MLV_COLOR_RED);
      MLV_update_window();
    }
    } else if (y >= BUTTON_Y_POS(3) && y <= BUTTON_Y_POS(3) + BUTTON_HEIGHT) {
    // Save Binary RGBA
    if (*tree != NULL) {
      DRAW_INPUT_BOX(3, "Enter filename to save (RGBA): ", &filename);
      save_quadtree_binary(*tree, filename);
      MLV_draw_text(200, 10, "Quadtree saved (RGBA)", MLV_COLOR_RED);
      MLV_update_window();
    }
    } else if (y >= BUTTON_Y_POS(4) && y <= BUTTON_Y_POS(4) + BUTTON_HEIGHT) {
    // Minimize Quadtree
    if (*tree != NULL) {
      minimize_identical_leaves_in_node(*tree);
      MLV_draw_text(200, 10, "Quadtree minimized", MLV_COLOR_RED);
      MLV_update_window();
    }
    } else if (y >= BUTTON_Y_POS(5) && y <= BUTTON_Y_POS(5) + BUTTON_HEIGHT) {
    // Save Minimized BW
    if (*tree != NULL) {
      DRAW_INPUT_BOX(5, "Enter filename to save (Minimized BW): ", &filename);
      save_quadtree_binary(*tree, filename);
      MLV_draw_text(200, 10, "Quadtree saved (Minimized BW)", MLV_COLOR_RED);
      MLV_update_window();
    }
    } else if (y >= BUTTON_Y_POS(6) && y <= BUTTON_Y_POS(6) + BUTTON_HEIGHT) {
    // Save Minimized RGBA
    if (*tree != NULL) {
      DRAW_INPUT_BOX(6, "Enter filename to save (Minimized RGBA): ", &filename);
      save_quadtree_binary(*tree, filename);
      MLV_draw_text(200, 10, "Quadtree saved (Minimized RGBA)", MLV_COLOR_RED);
      MLV_update_window();
    }
    } else if (y >= BUTTON_Y_POS(7) && y <= BUTTON_Y_POS(7) + BUTTON_HEIGHT) {
    // Open Image
    DRAW_INPUT_BOX(7, "Enter filename to open: ", &filename);
    *tree = load_quadtree_binary(filename);
    if (*tree != NULL) {
      MLV_draw_text(200, 10, "Quadtree loaded", MLV_COLOR_RED);
      MLV_update_window();
    }
    }
  }
  }
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

  return create_c_node(NULL, children);
}

void draw_c_tree_as_image_aux(c_node *c_tree, int x, int y, int length) {
  if (c_tree->children == NULL) {
    MLV_draw_filled_rectangle(x, y, length, length, MLV_rgba(c_tree->color->red, c_tree->color->green, c_tree->color->blue, c_tree->color->alpha));
  } else {
    draw_c_tree_as_image_aux(c_tree->children[0], x, y, length/2);
    draw_c_tree_as_image_aux(c_tree->children[1], x + length/2, y, length/2);
    draw_c_tree_as_image_aux(c_tree->children[2], x + length/2, y + length/2, length/2);
    draw_c_tree_as_image_aux(c_tree->children[3], x, y + length/2, length/2);
  }
}

void draw_c_tree_as_image(c_node *c_tree) {
  draw_c_tree_as_image_aux(c_tree, X_ORIGIN, Y_ORIGIN, 512);
  MLV_update_window();
}

c_node *approximate_image(MLV_Image *image) {
  c_node *tree = NULL;
  color *c = average_color(image, 0, 0, MLV_get_image_width(image), MLV_get_image_height(image));
  tree = create_c_leaf(c);
  draw_c_tree_as_image(tree);
  return tree;
}
