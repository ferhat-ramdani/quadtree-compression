#include <stdlib.h>
#include <stdbool.h>
#include "c_tree.h"
#include "minimizer.h"

void minimize_quadtree(c_node *node) {
  if (node->children == NULL) return;

  for (int i = 0; i < MAX_CHILDREN; i++) {
    minimize_quadtree(node->children[i]);
  }

  bool all_children_are_leaves = true;
  color *first_color = node->children[0]->color;
  for (int i = 0; i < MAX_CHILDREN; i++) {
    if (node->children[i]->children != NULL || node->children[i]->color != first_color) {
      all_children_are_leaves = false;
      break;
    }
  }

  if (all_children_are_leaves) {
    for (int i = 0; i < MAX_CHILDREN; i++) {
      free_c_tree(node->children[i]);
    }
    free(node->children);
    node->children = NULL;
    node->color = first_color;
  }
}