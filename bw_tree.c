#include <stdio.h>
#include <stdlib.h>
#include "bw_tree.h"

bw_node *create_bw_node(char data, bw_node **children) {
  bw_node *bw_tree = (bw_node *)malloc(sizeof(bw_node));
  bw_tree->data = data;
  bw_tree->children = children;
  return bw_tree;
}

bw_node *create_bw_leaf(char data) {
  bw_node *bw_tree = (bw_node *)malloc(sizeof(bw_node));
  bw_tree->data = data;
  bw_tree->children = NULL;
  return bw_tree;
}

void display_bw_tree(bw_node *t) {
  if (t == NULL) return;
  printf("%s ", 
    t->data == 1 ? "white" :  t->data == 0 ? "black" : "");
    printf("%s", t->children ? "\n->\n" : "");
  for (int i = 0; t->children &&  i < MAX_CHILDREN; i++) {
    display_bw_tree(t->children[i]);
  }
  printf("%s", t->children ? "\n<-\n" : "");
}

float bw_node_leaf_dist(bw_node *node, bw_node *leaf) {
  float d = 0.0;
  int i;
  for(i = 0; i < MAX_CHILDREN; i++) {
    d += bw_distance(node->children[i], leaf);
  }
  return d/4;
}

float bw_node_node_dist(bw_node *node1, bw_node *node2) {
  float d = 0.0;
  int i;
  for(i = 0; i < MAX_CHILDREN; i++) {
    d += bw_distance(node1->children[i], node2->children[i]);
  }
  return d/4;
}

float bw_distance(bw_node *t1, bw_node *t2) {
  if(t1 == NULL || t2 == NULL)
    return 0.0;
  if(t1->children == NULL && t2->children == NULL)
    return t1->data == t2->data ? 0 : 1;
  else if (t1->children != NULL && t2->children == NULL)
    return bw_node_leaf_dist(t1, t2);
  else if (t1->children == NULL && t2->children != NULL)
    return bw_node_leaf_dist(t2, t1);
  else
    return bw_node_node_dist(t1, t2);
}

void sub_tree_distances(bw_node *t1, bw_node *t2, float *distances) {
  /* TODO ... */
}

void free_bw_tree(bw_node *t) {
  if (t == NULL || !t->children) // if bw_tree is empty or leaf
    return;
  for (int i = 0; i < MAX_CHILDREN; i++)
    free_bw_tree(t->children[i]);
  free(t->children);
  free(t);
}
