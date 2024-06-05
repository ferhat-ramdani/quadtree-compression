#ifndef BW_TREE_H
#define BW_TREE_H

#ifndef MAX_CHILDREN
#define MAX_CHILDREN 4
#endif

typedef struct _bw_node {
  char data;                  /* stored data of type char */
  struct _bw_node **children; /* pointers to the children */
} bw_node;

typedef struct _bw_dist {
  bw_node *t1;
  bw_node *t2;
  float dist;
} bw_dist;

/*
* Creates a node with the given data and children.
* children must be allocated before calling this function.
*/
bw_node *create_bw_node(char data, bw_node **children);

/* Creates a leaf. */
bw_node *create_bw_leaf(char data);

/* Display the nodes of the bw_tree t */
void display_bw_tree(bw_node *t);

/* Measures distance between two trees, suppose t1 and t2 not NULL */
float bw_distance(bw_node *t1, bw_node *t2);

/* Stores the distances between every sub tree of the two trees in array distances */
void sub_tree_distances(bw_node *t1, bw_node *t2, float *distances);

/* 
 * Free the memory allocated for the tree t
 * Note: does not free memory for the leaves of the tree.
 */
void free_bw_tree(bw_node *t);

#endif /* BW_TREE_H */
