#ifndef BINARY_CONVERTER_H
#define BINARY_CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include "c_tree.h"
#include "bw_tree.h"

#define BUFFER_SIZE 8

typedef struct {
  unsigned char buffer;
  int bit_count;
  FILE *file;
} BitBuffer;

/* Save a c_tree to a binary file */
void save_c_tree_binary(const char *filename, c_node *c_tree);

/* Save a bw_tree to a binary file */
void save_bw_tree_binary(const char *filename, bw_node *bw_tree);

/* Load a c_tree from a binary file */
c_node *load_c_tree_binary(const char *filename);

/* Load a bw_tree from a binary file */
bw_node *load_bw_tree_binary(const char *filename);

#endif /* BINARY_CONVERTER_H */
