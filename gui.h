#ifndef GUI_H
#define GUI_H

#include <MLV/MLV_all.h>
#include "c_tree.h"

/* Macros */
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
#define CLEAR_NOTIFICATION() MLV_draw_filled_rectangle(MARGIN_LEFT + BUTTON_WIDTH, 0, WIDTH, HEIGHT/10, MLV_COLOR_BLACK)


/* Initialize the graphical window */
void initialize_window();

/* Draw buttons for different functionalities */
void draw_buttons();

/* Handle button clicks and perform corresponding actions */
void handle_buttons(MLV_Image **image, c_node **tree);

/* Calculate the average color of a rectangular area in an image */
color *average_color(MLV_Image *image, int x, int y, int width, int height);

/* Create a quadtree (c_tree) from an image */
c_node *create_c_tree_from_image(MLV_Image *image, int x, int y, int length);

/* Approximate an image using a quadtree */
c_node *approximate_image(MLV_Image *image, char shape);

#endif /* GUI_H */
