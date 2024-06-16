#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "c_tree.h"
#define HASH_TABLE_SIZE 1024

typedef struct HashNode {
  c_node *leaf;
  struct HashNode *next;
} HashNode;

typedef struct {
  HashNode *table[HASH_TABLE_SIZE];
} HashTable;
/* Create hashtable */
HashTable* create_hash_table();

/* Free the hashtable */
void free_hash_table(HashTable *hash_table);

/* Minimize a quadtree by combining identical leaves */
void minimize_identical_leaves_in_node(c_node *node);

/* Minimize a quadtree by keeping only one leaf per color, stores the leaves in given hashtable */
c_node *minimize_unique_leaves(c_node *node, HashTable *hash_table);

#endif /* MINIMIZER_H */
