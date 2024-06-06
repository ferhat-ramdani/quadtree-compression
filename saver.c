#include <stdio.h>
#include <stdlib.h>
#include "c_tree.h"
#include "bw_tree.h"

void save_c_node(FILE *file, c_node *node) {
  if (node->children == NULL) {
    unsigned char marker = 0x1;
    fwrite(&marker, sizeof(unsigned char), 1, file);
    fwrite(&(node->color->red), sizeof(unsigned char), 1, file);
    fwrite(&(node->color->green), sizeof(unsigned char), 1, file);
    fwrite(&(node->color->blue), sizeof(unsigned char), 1, file);
    fwrite(&(node->color->alpha), sizeof(unsigned char), 1, file);
  } else {
    unsigned char marker = 0;
    fwrite(&marker, sizeof(unsigned char), 1, file);
    for (int i = 0; i < MAX_CHILDREN; i++) {
      save_c_node(file, node->children[i]);
    }
  }
}

void save_bw_node(FILE *file, bw_node *node) {
  if (node->children == NULL) {
    char marker = 1;
    fwrite(&marker, sizeof(char), 1, file);
    fwrite(&(node->data), sizeof(char), 1, file);
  } else {
    char marker = 0;
    fwrite(&marker, sizeof(char), 1, file);
    for (int i = 0; i < MAX_CHILDREN; i++) {
      save_bw_node(file, node->children[i]);
    }
  }
}

c_node* load_c_node(FILE *file) {
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
      children[i] = load_c_node(file);
    }
    return create_c_node(children);
  }
}

bw_node* load_bw_node(FILE *file) {
  char marker;
  fread(&marker, sizeof(char), 1, file);
  if (marker == 1) {
    char data;
    fread(&data, sizeof(char), 1, file);
    return create_bw_leaf(data);
  } else {
    bw_node **children = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
    for (int i = 0; i < MAX_CHILDREN; i++) {
      children[i] = load_bw_node(file);
    }
    return create_bw_node(0, children);
  }
}

void save_c_tree_binary(const char *filename, c_node *c_tree) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }
  save_c_node(file, c_tree);
  fclose(file);
}

void save_bw_tree_binary(const char *filename, bw_node *bw_tree) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }
  save_bw_node(file, bw_tree);
  fclose(file);
}

c_node *load_c_tree_binary(const char *filename) {
  FILE *file = fopen(filename, "rb"); // to read the file in a binary method
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  c_node *tree = load_c_node(file);
  fclose(file);
  return tree;
}

bw_node *load_bw_tree_binary(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  bw_node *tree = load_bw_node(file);
  fclose(file);
  return tree;
}
