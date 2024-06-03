#include <stdio.h>
#include <stdlib.h>
#include "bw_tree.h"
#include "c_tree.h"
#include "color.h"
#include "gui.h"

// int main() {
//   color *green = create_color(0, 255, 0, 0);
//   color *red = create_color(255, 0, 0, 0);
//   color *blue = create_color(0, 0, 255, 0);
//   color *yellow = create_color(255, 255, 0, 0);

//   c_node *leaf1 = create_c_leaf(green);
//   c_node *leaf2 = create_c_leaf(red);
//   c_node *leaf3 = create_c_leaf(blue);
//   c_node *leaf4 = create_c_leaf(yellow);

//   c_node **children1 = (c_node **)malloc(4 * sizeof(c_node *));
//   children1[0] = leaf1;
//   children1[1] = leaf2;
//   children1[2] = leaf3;
//   children1[3] = leaf4;

//   c_node **children2 = (c_node **)malloc(4 * sizeof(c_node *));
//   children2[0] = leaf2;
//   children2[1] = leaf2;
//   children2[2] = leaf1;
//   children2[3] = leaf1;

//   c_node *node1 = create_c_node(NULL, children1);
//   c_node *node2 = create_c_node(NULL, children2);

//   c_node **children3 = (c_node **)malloc(4 * sizeof(c_node *));
//   children3[0] = node1;
//   children3[1] = leaf3;
//   children3[2] = leaf2;
//   children3[3] = node2;

//   c_node *root = create_c_node(NULL, children3);

//   display_c_tree(root);

//   free_c_tree(root);

//   free(leaf1);
//   free(leaf2);
//   free(leaf3);
//   free(leaf4);

//   free(green);
//   free(red);
//   free(blue);
//   free(yellow);

//   return 0;
// }

int main() {
    MLV_Image *image = NULL;
    c_node *tree = NULL;

    initialize_window();
    draw_buttons();
    MLV_update_window();

    handle_buttons(&image, &tree);

    MLV_free_window();
    return 0;
}