#include <stdio.h>
#include <stdlib.h>
#include "bw_tree.h"
#include "c_tree.h"
#include "color.h"
#include "gui.h"
#include "saver.h"

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
    return 0;
}