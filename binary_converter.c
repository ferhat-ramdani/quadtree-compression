#include <stdio.h>
#include <stdlib.h>
#include "c_tree.h"
#include "bw_tree.h"
#include "binary_converter.h"

void init_bit_buffer(BitBuffer *bit_buffer, FILE *file) {
  bit_buffer->buffer = 0;
  bit_buffer->bit_count = 0;
  bit_buffer->file = file;
}

void flush_bit_buffer(BitBuffer *bit_buffer) {
  if (bit_buffer->bit_count > 0) {
    fwrite(&bit_buffer->buffer, sizeof(unsigned char), 1, bit_buffer->file);
    bit_buffer->buffer = 0;
    bit_buffer->bit_count = 0;
  }
}

void write_bit(BitBuffer *bit_buffer, int bit) {
  bit_buffer->buffer <<= 1;
  if (bit) {
    bit_buffer->buffer |= 1;
  }
  bit_buffer->bit_count++;
  if (bit_buffer->bit_count == BUFFER_SIZE) {
    flush_bit_buffer(bit_buffer);
  }
}

int read_bit(BitBuffer *bit_buffer) {
  if (bit_buffer->bit_count == 0) {
    fread(&bit_buffer->buffer, sizeof(unsigned char), 1, bit_buffer->file);
    bit_buffer->bit_count = BUFFER_SIZE;
  }
  int bit = (bit_buffer->buffer >> (bit_buffer->bit_count - 1)) & 1;
  bit_buffer->bit_count--;
  return bit;
}

void write_byte(BitBuffer *bit_buffer, unsigned char byte) {
  for (int i = 0; i < 8; i++) {
    write_bit(bit_buffer, (byte >> (7 - i)) & 1);
  }
}

unsigned char read_byte(BitBuffer *bit_buffer) {
  unsigned char byte = 0;
  for (int i = 0; i < 8; i++) {
    byte <<= 1;
    byte |= read_bit(bit_buffer);
  }
  return byte;
}

void save_c_node(BitBuffer *bit_buffer, c_node *node) {
  if (node->children == NULL) {
    write_bit(bit_buffer, 1);
    write_byte(bit_buffer, node->color->red);
    write_byte(bit_buffer, node->color->green);
    write_byte(bit_buffer, node->color->blue);
    write_byte(bit_buffer, node->color->alpha);
  } else {
    write_bit(bit_buffer, 0);
    for (int i = 0; i < MAX_CHILDREN; i++) {
      save_c_node(bit_buffer, node->children[i]);
    }
  }
  int bit = (bit_buffer->buffer >> (bit_buffer->bit_count - 1)) & 1;
  bit_buffer->bit_count--;
  return bit;
}

void save_bw_node(BitBuffer *bit_buffer, bw_node *node) {
  if (node->children == NULL) {
    write_bit(bit_buffer, 1);
    write_byte(bit_buffer, node->data);
  } else {
    write_bit(bit_buffer, 0);
    for (int i = 0; i < MAX_CHILDREN; i++) {
      save_bw_node(bit_buffer, node->children[i]);
    }
  }
}

// void save_bw_node(FILE *file, bw_node *node) {
//   if (node->children == NULL) {
//     char marker = 1;
//     fwrite(&marker, sizeof(char), 1, file);
//     fwrite(&(node->data), sizeof(char), 1, file);
//   } else {
//     char marker = 0;
//     fwrite(&marker, sizeof(char), 1, file);
//     for (int i = 0; i < MAX_CHILDREN; i++) {
//       save_bw_node(file, node->children[i]);
//     }
//   }
// }

c_node* load_c_node(BitBuffer *bit_buffer) {
  int marker = read_bit(bit_buffer);
  if (marker == 1) {
    unsigned char red = read_byte(bit_buffer);
    unsigned char green = read_byte(bit_buffer);
    unsigned char blue = read_byte(bit_buffer);
    unsigned char alpha = read_byte(bit_buffer);
    color *c = create_color(red, green, blue, alpha);
    return create_c_leaf(c);
  } else {
    c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
    for (int i = 0; i < MAX_CHILDREN; i++) {
      children[i] = load_c_node(bit_buffer);
    }
    return create_c_node(children);
  }
}

bw_node* load_bw_node(BitBuffer *bit_buffer) {
  int marker = read_bit(bit_buffer);
  if (marker == 1) {
    unsigned char data = read_byte(bit_buffer);
    return create_bw_leaf(data);
  } else {
    bw_node **children = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
    for (int i = 0; i < MAX_CHILDREN; i++) {
      children[i] = load_bw_node(bit_buffer);
    }
    return create_bw_node(0, children);
  }
}

// bw_node* load_bw_node(FILE *file) {
//   char marker;
//   fread(&marker, sizeof(char), 1, file);
//   if (marker == 1) {
//     char data;
//     fread(&data, sizeof(char), 1, file);
//     return create_bw_leaf(data);
//   } else {
//     bw_node **children = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
//     for (int i = 0; i < MAX_CHILDREN; i++) {
//       children[i] = load_bw_node(file);
//     }
//     return create_bw_node(0, children);
//   }
// }
void save_c_tree_binary(const char *filename, c_node *tree) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }
  BitBuffer bit_buffer;
  init_bit_buffer(&bit_buffer, file);
  save_c_node(&bit_buffer, tree);
  flush_bit_buffer(&bit_buffer);
  fclose(file);
}

void save_bw_tree_binary(const char *filename, bw_node *bw_tree) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }
  BitBuffer bit_buffer;
  init_bit_buffer(&bit_buffer, file);
  save_bw_node(&bit_buffer, bw_tree);
  flush_bit_buffer(&bit_buffer);
  fclose(file);
}

// void save_bw_tree_binary(const char *filename, bw_node *bw_tree) {
//   FILE *file = fopen(filename, "wb");
//   if (!file) {
//     perror("Error opening file");
//     return;
//   }
//   save_bw_node(file, bw_tree);
//   fclose(file);
// }

c_node* load_c_tree_binary(const char *filename) {
  FILE *file = fopen(filename, "rb"); // to read the file in a binary method
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  BitBuffer bit_buffer;
  init_bit_buffer(&bit_buffer, file);
  c_node *tree = load_c_node(&bit_buffer);
  // c_node *tree = load_c_node(file);
  fclose(file);
  return tree;
}

bw_node *load_bw_tree_binary(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  BitBuffer bit_buffer;
  init_bit_buffer(&bit_buffer, file);
  bw_node *tree = load_bw_node(&bit_buffer);
  fclose(file);
  return tree;
}

// bw_node *load_bw_tree_binary(const char *filename) {
//   FILE *file = fopen(filename, "rb");
//   if (!file) {
//     perror("Error opening file");
//     return NULL;
//   }
//   bw_node *tree = load_bw_node(file);
//   fclose(file);
//   return tree;
// }
