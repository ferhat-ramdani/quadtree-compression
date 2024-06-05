#ifndef BINARY
#define BINARY

#include "c_tree.h"

/* Save a quadtree to a binary file */
void save_quadtree_binary(c_node *tree, const char *filename);

/* Load a quadtree from a binary file */
c_node* load_quadtree_binary(const char *filename);


#endif /* BINARY */
