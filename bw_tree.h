#ifndef BW_TREE_H
#define BW_TREE_H

#define MAX_CHILDREN 4

typedef struct _bw_node {
    char data;                /* stored data of type char */
    struct _bw_node **children; /* pointers to the children */
} bw_node;

/*
* Creates a node with the given data and children.
* children must be allocated before calling this function.
*/
bw_node *create_node(char data, bw_node **children);

/* Creates a leaf. */
bw_node *create_leaf(char data);

/* Display the nodes of the bw_tree t */
void display_bw_tree(bw_node *t);

/* Measures distance between two trees, suppose t1 and t2 not NULL */
float distance(bw_node *t1, bw_node *t2);

/* Free the memory allocated for the tree t */
void free_bw_tree(bw_node *t);

#endif /* BW_TREE_H */
