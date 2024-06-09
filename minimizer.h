#ifndef MINIMIZER
#define MINIMIZER

#include "c_tree.h"
#define HASH_TABLE_SIZE 1024

typedef struct HashNode {
    color *color;
    c_node *leaf;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *table[HASH_TABLE_SIZE];
} HashTable;

/* Minimize a quadtree by combining identical leaves */
void minimize_identical_leaves_in_node(c_node *node);

/* Minimize a quadtree by keeping only one leaf per color */
void minimize_unique_leaves(c_node *node);

#endif /* MINIMIZER */
