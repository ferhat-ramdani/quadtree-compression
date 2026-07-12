#include <stdio.h>
#include <stdlib.h>
#include "bw_tree.h"
#include "c_tree.h"
#include "color.h"
#include "gui.h"
#include "binary_converter.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    MLV_Image *image = NULL;
    c_node *tree = NULL;
    
    printf("Starting program...\n");
    fflush(stdout);

    initialize_window();
    draw_buttons();
    MLV_update_window();

    handle_buttons(&image, &tree);

    MLV_free_window();

    free_c_tree(tree);
    free(image);
    return 0;
}