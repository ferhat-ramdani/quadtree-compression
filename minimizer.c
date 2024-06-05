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
    if (node->children[i]->children != NULL || node->children[i]->color != first_color) {
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