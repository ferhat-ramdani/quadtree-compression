#ifndef GUI_H
#define GUI_H

#include <MLV/MLV_all.h>
#include "c_tree.h"

/* Initialize the graphical window */
void initialize_window();

/* Draw buttons for different functionalities */
void draw_buttons();

/* Handle button clicks and perform corresponding actions */
void handle_buttons(MLV_Image **image, c_node **tree);

/* Calculate the average color of a rectangular area in an image */
color *average_color(MLV_Image *image, int x, int y, int width, int height);

/* Create a quadtree (c_tree) from an image */
c_node *create_c_tree_from_image(MLV_Image *image, int x, int y, int width, int height);

/* Save a quadtree to a binary file */
void save_quadtree_binary(c_node *tree, const char *filename);

/* Minimize a quadtree */
void minimize_quadtree(c_node *node);

/* Load a quadtree from a binary file */
c_node* load_quadtree_binary(const char *filename);

#endif /* GUI_H */
