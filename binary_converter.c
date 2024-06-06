#include <stdio.h>
#include <stdlib.h>
#include "c_tree.h"

void save_node(FILE *file, c_node *node) {
  if (node->children == NULL) {
    unsigned char marker = 1;
    fwrite(&marker, sizeof(unsigned char), 1, file);
    fwrite(&(node->color->red), sizeof(unsigned char), 1, file);
    fwrite(&(node->color->green), sizeof(unsigned char), 1, file);
    fwrite(&(node->color->blue), sizeof(unsigned char), 1, file);
    fwrite(&(node->color->alpha), sizeof(unsigned char), 1, file);
  } else {
    unsigned char marker = 0;
    fwrite(&marker, sizeof(unsigned char), 1, file);
    for (int i = 0; i < MAX_CHILDREN; i++) {
      save_node(file, node->children[i]);
    }
  }
}

c_node* load_node(FILE *file) {
  unsigned char marker;
  fread(&marker, sizeof(unsigned char), 1, file);
  if (marker == 1) {
    unsigned char red, green, blue, alpha;
    fread(&red, sizeof(unsigned char), 1, file);
    fread(&green, sizeof(unsigned char), 1, file);
    fread(&blue, sizeof(unsigned char), 1, file);
    fread(&alpha, sizeof(unsigned char), 1, file);
    color *c = create_color(red, green, blue, alpha);
    return create_c_leaf(c);
  } else {
    c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
    for (int i = 0; i < MAX_CHILDREN; i++) {
      children[i] = load_node(file);
    }
    return create_c_node(children);
  }
}

void save_c_tree_binary(const char *filename, c_node *c_tree) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }
  save_node(file, c_tree);
  fclose(file);
}

c_node* load_c_tree_binary(const char *filename) {
  FILE *file = fopen(filename, "rb"); // to read the file in a binary method
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  c_node *tree = load_node(file);
  fclose(file);
  return tree;
}
