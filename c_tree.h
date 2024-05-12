#ifndef C_TREE_H
#define C_TREE_H

#include "color.h"

#ifndef MAX_CHILDREN
#define MAX_CHILDREN 4
#endif

typedef struct _c_node {
  color *color;              /* stored data of type char */
  struct _c_node **children; /* pointers to the children */
} c_node;

/*
* Creates a node with the given data and children.
* children must be allocated before calling this function.
*/
c_node *create_c_node(color *color, c_node **children);

/* Creates a leaf. */
c_node *create_c_leaf(color *color);

/* Display the nodes of the c_tree t */
void display_c_tree(c_node *t);

/* Measures distance between two trees, suppose t1 and t2 not NULL */
float c_distance(c_node *t1, c_node *t2);

/* 
 * Free the memory allocated for the tree t
 * Note: does not free memory for the leaves of the tree.
 */
void free_c_tree(c_node *t);

#endif /* C_TREE_H */
