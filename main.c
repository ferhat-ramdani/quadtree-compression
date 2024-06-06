#include <stdio.h>
#include <stdlib.h>
#include "bw_tree.h"
#include "c_tree.h"
#include "color.h"
#include "gui.h"
#include "binary_converter.h"

int main() {
    MLV_Image *image = NULL;
    c_node *tree = NULL;

    initialize_window();
    draw_buttons();
    MLV_update_window();

    handle_buttons(&image, &tree);

    MLV_free_window();

    free_c_tree(tree);
    free(image);

    // // create 4 colors
    // color *c1 = create_color(23, 123, 3, 255);
    // color *c2 = create_color(82, 192, 27, 255);
    // color *c3 = create_color(32, 12, 233, 255);
    // color *c4 = create_color(82, 23, 72, 255);

    // // create 4 leaves
    // c_node *leaf1 = create_c_leaf(c1);
    // c_node *leaf2 = create_c_leaf(c2);
    // c_node *leaf3 = create_c_leaf(c3);
    // c_node *leaf4 = create_c_leaf(c4);

    // // create an array of 4 children
    // c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
    // children[0] = leaf1;
    // children[1] = leaf2;
    // children[2] = leaf3;
    // children[3] = leaf4;

    // // create a c_node with the previous children
    // c_node *c_tree = create_c_node(children);

    // // display the c_tree
    // display_c_tree(c_tree);

    // printf("\n\ndone\n\n");

    // // save the c_tree in a binary file
    // save_c_tree_binary("c.qtc", c_tree);

    // // load the c_tree from the binary file
    // c_node *loaded_tree = load_c_tree_binary("c.qtc");

    // // display the loaded tree
    // display_c_tree(loaded_tree);
    return 0;
}