#ifndef BINARY
#define BINARY

#include "c_tree.h"

/* Save a quadtree to a binary file */
void save_c_tree_binary(const char *filename, c_node *tree);

/* Load a quadtree from a binary file */
c_node* load_c_tree_binary(const char *filename);


#endif /* BINARY */
