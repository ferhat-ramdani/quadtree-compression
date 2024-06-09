#include "binary_converter.h"

#define BUFFER_SIZE 8


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

void save_node(BitBuffer *bit_buffer, c_node *node) {
  if (node->children == NULL) {
    write_bit(bit_buffer, 1);
    write_byte(bit_buffer, node->color->red);
    write_byte(bit_buffer, node->color->green);
    write_byte(bit_buffer, node->color->blue);
    write_byte(bit_buffer, node->color->alpha);
  } else {
    write_bit(bit_buffer, 0);
    for (int i = 0; i < MAX_CHILDREN; i++) {
      save_node(bit_buffer, node->children[i]);
    }
  }
}

c_node* load_node(BitBuffer *bit_buffer) {
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
      // children[i] = load_node(file);
      children[i] = load_node(bit_buffer);
    }
    return create_c_node(children);
  }
}

void save_quadtree_binary(c_node *tree, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }
  BitBuffer bit_buffer;
  init_bit_buffer(&bit_buffer, file);
  // save_node(file, tree);
  save_node(&bit_buffer, tree);
  flush_bit_buffer(&bit_buffer);
  fclose(file);
}

c_node* load_quadtree_binary(const char *filename) {
  FILE *file = fopen(filename, "rb"); // to read the file in a binary method
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  BitBuffer bit_buffer;
  init_bit_buffer(&bit_buffer, file);
  c_node *tree = load_node(&bit_buffer);
  // c_node *tree = load_node(file);
  fclose(file);
  return tree;
}
