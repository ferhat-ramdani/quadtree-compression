#include <stdlib.h>
#include <stdbool.h>
#include "c_tree.h"
#include "minimizer.h"

void minimize_identical_leaves_in_node(c_node *node) {
  if (node->children == NULL) return;

  for (int i = 0; i < MAX_CHILDREN; i++) {
    minimize_identical_leaves_in_node(node->children[i]);
  }

  bool identical_leaves_in_node = true;
  color *first_color = node->children[0]->color;
  for (int i = 1; i < MAX_CHILDREN; i++) {
    if (node->children[i]->children != NULL ||  
        node->children[i]->color->red != first_color->red || 
        node->children[i]->color->green != first_color->green || 
        node->children[i]->color->blue != first_color->blue || 
        node->children[i]->color->alpha != first_color->alpha) {
      identical_leaves_in_node = false;
      break;
    }
  }

  if (identical_leaves_in_node) {
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
    return (c->red * 31 + c->green * 37 + c->blue * 41 + c->alpha * 43) % HASH_TABLE_SIZE;
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
    unsigned int hash = hash_color(c);
    HashNode *node = hash_table->table[hash];
    while (node) {
        if (memcmp(node->leaf->color, c, sizeof(color)) == 0) {
            return node->leaf;
        }
        node = node->next;
    }
    return NULL;
}

void add_leaf(HashTable *hash_table, color *c, c_node *leaf) {
    unsigned int hash = hash_color(c);
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    new_node->leaf = leaf;
    new_node->next = hash_table->table[hash];
    hash_table->table[hash] = new_node;
}

c_node* minimize_unique_leaves_aux(c_node *node, HashTable *hash_table) {
    if (node->children == NULL) {
        c_node *existing_leaf = find_leaf(hash_table, node->color);
        if (existing_leaf != NULL) {
            free_c_leaf(node);
            return existing_leaf;
        } else {
            return node;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = minimize_unique_leaves_aux(node->children[i], hash_table);
    }
    return node;
}

c_node* minimize_unique_leaves(c_node *node) {
    HashTable *hash_table = create_hash_table();
    c_node *new_root = minimize_unique_leaves_aux(node, hash_table);
    free_hash_table(hash_table);
    return new_root;
}
