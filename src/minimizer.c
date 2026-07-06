#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "c_tree.h"
#include "minimizer.h"
#include "color.h"

void minimize_identical_leaves_in_node(c_node *node) {
  if (node->children == NULL) return;

  for (int i = 0; i < MAX_CHILDREN; i++) {
    minimize_identical_leaves_in_node(node->children[i]);
  }

  bool identical_leaves_in_node = true;
  if (node->children[0]->children != NULL) {
    identical_leaves_in_node = false;
  } else {
    color *first_color = node->children[0]->color;
    for (int i = 1; i < MAX_CHILDREN; i++) {
      if (node->children[i]->children != NULL || !c_equals(node->children[i]->color, first_color)) {
        identical_leaves_in_node = false;
        break;
      }
    }
  }

  if (identical_leaves_in_node) {
    color *first_color = node->children[0]->color;
    node->children[0]->color = NULL; // prevent free
    for (int i = 0; i < MAX_CHILDREN; i++) {
      free_c_tree(node->children[i]);
    }
    free(node->children);
    node->children = NULL;
    node->color = first_color;
  }
}

unsigned int hash_color(const color *c) {
  // To reduce hash collisions :
  return (c->red * 31 + c->green * 37 + c->blue * 41 + c->alpha * 43);
}

HashTable* create_hash_table() {
  HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));
  memset(hash_table->table, 0, sizeof(hash_table->table));
  return hash_table;
}

void free_hash_table(HashTable *hash_table) {
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    HashNode *node = hash_table->table[i];
    while (node) {
      HashNode *tmp = node;
      node = node->next;
      free(tmp);
    }
  }
  free(hash_table);
}

c_node* find_leaf(HashTable *hash_table, const color *c) {
  unsigned int index = hash_color(c) % HASH_TABLE_SIZE;
  HashNode *node = hash_table->table[index];
  while (node) {
    if (c_equals(node->leaf->color, c)) {
      return node->leaf;
    }
    node = node->next;
  }
  return NULL;
}

void add_leaf(HashTable *hash_table, c_node *leaf) {
  assert(leaf != NULL && leaf->color != NULL && leaf->children == NULL);
  unsigned int index = hash_color(leaf->color) % HASH_TABLE_SIZE;
  HashNode *hash_node = (HashNode *)malloc(sizeof(HashNode));
  hash_node->leaf = leaf;
  hash_node->next = hash_table->table[index];
  hash_table->table[index] = hash_node;
}

c_node *minimize_unique_leaves(c_node *node, HashTable *hash_table) {
  if (node->children == NULL) {
    c_node *existing_leaf = find_leaf(hash_table, node->color);
    if (existing_leaf != NULL) {
      free_c_leaf(node);
      return existing_leaf;
    } else {
      add_leaf(hash_table, node);
      return node;
    }
  }

  for (int i = 0; i < MAX_CHILDREN; i++) {
    node->children[i] = minimize_unique_leaves(node->children[i], hash_table);
  }
  return node;
}
