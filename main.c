#include <stdio.h>
#include <stdlib.h>
#include "bw_tree.h"

int main() {

  bw_node *black = create_leaf(0);
  bw_node *white = create_leaf(1);

  bw_node **children1 = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
  bw_node **children2 = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
  bw_node **children3 = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
  bw_node **children4 = (bw_node **)malloc(MAX_CHILDREN * sizeof(bw_node *));
  
  children1[0] = black;
  children1[1] = white;
  children1[2] = NULL;
  children1[3] = black;

  children2[0] = white;
  children2[1] = NULL;
  children2[2] = NULL;
  children2[3] = white;

  bw_node *node1 = create_node(2, children1);
  bw_node *node2 = create_node(2, children2);

  children3[0] = node1;
  children3[2] = node2;

  children4[0] = white;
  children4[1] = white;
  children4[2] = white;
  children4[3] = white;

  bw_node *root2 = create_node(2, children4);
  bw_node *root1 = create_node(2, children3);

  printf("root1:\n");
  display_bw_tree(root1);

  printf("\nroot2:\n");
  display_bw_tree(root2);

  printf("\nDistance between tree1 and tree2: %f\n", distance(root1, root2));

  free_bw_tree(root1);
  free_bw_tree(root2);

  free(black);
  free(white);

  return 0;
}