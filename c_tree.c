#include <stdlib.h>
#include <stdio.h>
#include "c_tree.h"

c_node *create_c_node(color *color, c_node **children) {
  c_node *c_tree = (c_node *)malloc(sizeof(c_node));
  c_tree->color = color;
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
  for (int i = 0; t->children &&  i < MAX_CHILDREN; i++) {
    display_c_tree(t->children[i]);
  }
  printf("%s", t->children ? "\n<-\n" : "");
}

float c_node_leaf_dist(c_node *node, c_node *leaf) {
  float d = 0.0;
  int i;
  for(i = 0; i < MAX_CHILDREN; i++) {
    d += c_distance(node->children[i], leaf);
  }
  return d/4;
}

float c_node_node_dist(c_node *node1, c_node *node2) {
  float d = 0.0;
  int i;
  for(i = 0; i < MAX_CHILDREN; i++) {
    d += c_distance(node1->children[i], node2->children[i]);
  }
  return d/4;
}

float c_distance(c_node *t1, c_node *t2) {
  if(t1 == NULL || t2 == NULL)
    return 0.0;
  if(t1->children == NULL && t2->children == NULL)
    return c_c_distance(*(t1->color), *(t2->color));
  else if (t1->children != NULL && t2->children == NULL)
    return c_node_leaf_dist(t1, t2);
  else if (t1->children == NULL && t2->children != NULL)
    return c_node_leaf_dist(t2, t1);
  else
    return c_node_node_dist(t1, t2);
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
