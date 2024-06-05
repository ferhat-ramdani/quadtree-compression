#include <stdlib.h>
#include <stdio.h>
#include "c_tree.h"

c_node *create_c_node(c_node **children) {
  c_node *c_tree = (c_node *)malloc(sizeof(c_node));
  c_tree->color = mean_color(children);
  c_tree->children = children;
  return c_tree;
}

c_node *create_c_leaf(color *color) {
  c_node *c_tree = (c_node *)malloc(sizeof(c_node));
  c_tree->color = color;
  c_tree->children = NULL;
  return c_tree;
}

void display_c_tree(c_node *t) {
  if (t == NULL) return;
  if(t->color) print_color(*(t->color));
  printf("%s", t->children ? "\n->\n" : "");
  for (int i = 0; t->children &&  i < MAX_CHILDREN; i++)
    display_c_tree(t->children[i]);
  printf("%s", t->children ? "\n<-\n" : "");
}

float c_node_leaf_dist(c_node *node, c_node *leaf) {
  float d = 0.0;
  int i;
  for(i = 0; i < MAX_CHILDREN; i++)
    d += c_exact_distance(node->children[i], leaf);
  return d/4;
}

float c_node_node_dist(c_node *node1, c_node *node2) {
  float d = 0.0;
  int i;
  for(i = 0; i < MAX_CHILDREN; i++) {
    d += c_exact_distance(node1->children[i], node2->children[i]);
  }
  return d/4;
}

float c_exact_distance(c_node *t1, c_node *t2) {
  if(t1 == NULL || t2 == NULL)
    return 0.0;
  if(t1->children == NULL && t2->children == NULL)
    return c_c_distance(*(t1->color), *(t2->color));
  if (t1->children != NULL && t2->children == NULL)
    return c_node_leaf_dist(t1, t2);
  if (t1->children == NULL && t2->children != NULL)
    return c_node_leaf_dist(t2, t1);
  else
    return c_node_node_dist(t1, t2);
}

float c_mean_distance(c_node *t1, c_node *t2) {
  if(t1 == NULL || t2 == NULL)
    return 0.0;
  if (t1 && !t2) return c_distance(*(t1->color));
  if (t2 && !t1) return c_distance(*(t2->color));
  return c_c_distance(*(t1->color), *(t2->color));
}

color *mean_color(c_node **children) {
  int i, r = 0, g = 0, b = 0, a = 0;
  for(i = 0; i < MAX_CHILDREN; i++) {
    if(children[i] == NULL)
      continue;
    r += children[i]->color->red = r;
    g += children[i]->color->green = g;
    b += children[i]->color->blue = b;
    a += children[i]->color->alpha = a;
  }
  return create_color(r/4, g/4, b/4, a/4);
}

// float *sub_c_tree_distances(c_node *t1, c_node *t2) {
//   if(t1 == NULL || t2 == NULL)
//     return NULL;
//   if(t1->children == NULL && t2->children == NULL) {
//     distances[0] = c_exact_distance(t1, t2);
//     return;
//   }
//   if(t1->children != NULL && t2->children == NULL) {
//     for(int i = 0; i < MAX_CHILDREN; i++)
//       sub_c_tree_distances(t1->children[i], t2, distances + i);
//     return;
//   }
//   if(t1->children == NULL && t2->children != NULL) {
//     for(int i = 0; i < MAX_CHILDREN; i++)
//       sub_c_tree_distances(t1, t2->children[i], distances + i);
//     return;
//   }
//   for(int i = 0; i < MAX_CHILDREN; i++)
//     sub_c_tree_distances(t1->children[i], t2->children[i], distances + i);
// }

void free_c_leaf(c_node *leaf) {
  if (leaf == NULL)
    return;
  if(leaf->color)
    free(leaf->color);
  free(leaf);
}

void free_c_tree(c_node *t) {
  if (t == NULL || !t->children) // if c_tree is empty or leaf
    return;
  for (int i = 0; i < MAX_CHILDREN; i++)
    free_c_tree(t->children[i]);
  free(t->color);
  free(t->children);
  free(t);
}
