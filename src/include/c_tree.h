#ifndef C_TREE_H
#define C_TREE_H

#include "color.h"

#ifndef MAX_CHILDREN
#define MAX_CHILDREN 4
#endif

typedef struct _c_node {
  color *color;              /* color of the node (mean color for internal nodes) */
  struct _c_node **children; /* pointers to the children of node */
} c_node;

/*
* Creates a node with the given children.
* The color of the returned node is the mean color of the colors of the children.
* children must be allocated before calling this function.
*/
c_node *create_c_node(c_node **children);

/* Creates a leaf with the given color. */
c_node *create_c_leaf(color *color);

/* Display the tree */
void display_c_tree(c_node *t);

/* 
* Measures exact distance between two trees, suppose t1 and t2 not NULL .
* The exact distance is the distance measured using the formulas given in the paper.
*/
float c_exact_distance(c_node *t1, c_node *t2);

/*
* Measures mean distance between two trees, suppose t1 and t2 not NULL.
* The mean distance is the distance measured using the mean color of the nodes of the two trees,
* this means that the distance between t1 and t2 is simply the distance between t1->color and t2->color.
*/
float c_mean_distance(c_node *t1, c_node *t2);

/* Returns the mean color of the colors of the given children nodes */
color *mean_color(c_node **children);

/* Stores the distances between every sub tree of the two trees in array distances */
// float *sub_c_tree_distances(c_node *t1, c_node *t2,);


/* Frees the leaf */
void free_c_leaf(c_node *leaf);

/* 
 * Free the memory allocated for the tree t
 * Note: does not free memory for the leaves of the tree.
 */
void free_c_tree(c_node *t);

#endif /* C_TREE_H */
