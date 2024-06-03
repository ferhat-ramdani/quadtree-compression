#include <MLV/MLV_all.h>
#include "gui.h"
#include "c_tree.h"
#include "color.h"
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

void initialize_window() {
    MLV_create_window("Quadtree Compression", "Quadtree Compression", WIDTH, HEIGHT);
}

void draw_buttons() {
    MLV_draw_text_box(10, 10, 180, 50, "Load Image", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 70, 180, 50, "Quadtree Approximation", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 130, 180, 50, "Save Binary BW", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 190, 180, 50, "Save Binary RGBA", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 250, 180, 50, "Minimize Quadtree", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 310, 180, 50, "Save Minimized BW", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 370, 180, 50, "Save Minimized RGBA", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(10, 430, 180, 50, "Open Image", 1, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_update_window();
}

void save_node(FILE *file, c_node *node) {
    if (node->children == NULL) {
        unsigned char marker = 1;
        fwrite(&marker, sizeof(unsigned char), 1, file);
        fwrite(&(node->color->red), sizeof(unsigned char), 1, file);
        fwrite(&(node->color->green), sizeof(unsigned char), 1, file);
        fwrite(&(node->color->blue), sizeof(unsigned char), 1, file);
        fwrite(&(node->color->alpha), sizeof(unsigned char), 1, file);
    } else {
        unsigned char marker = 0;
        fwrite(&marker, sizeof(unsigned char), 1, file);
        for (int i = 0; i < MAX_CHILDREN; i++) {
            save_node(file, node->children[i]);
        }
    }
}

void save_quadtree_binary(c_node *tree, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }
    save_node(file, tree);
    fclose(file);
}

c_node* load_node(FILE *file) {
    unsigned char marker;
    fread(&marker, sizeof(unsigned char), 1, file);
    if (marker == 1) {
        unsigned char red, green, blue, alpha;
        fread(&red, sizeof(unsigned char), 1, file);
        fread(&green, sizeof(unsigned char), 1, file);
        fread(&blue, sizeof(unsigned char), 1, file);
        fread(&alpha, sizeof(unsigned char), 1, file);
        color *c = create_color(red, green, blue, alpha);
        return create_c_leaf(c);
    } else {
        c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
        for (int i = 0; i < MAX_CHILDREN; i++) {
            children[i] = load_node(file);
        }
        return create_c_node(NULL, children);
    }
}

c_node* load_quadtree_binary(const char *filename) {
    FILE *file = fopen(filename, "rb"); // to read the file in a binary method
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    c_node *tree = load_node(file);
    fclose(file);
    return tree;
}

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

void handle_buttons(MLV_Image **image, c_node **tree) {
    int x, y;
    char *filename = NULL;

    while (1) {
        MLV_wait_mouse(&x, &y);
        
        if (x >= 10 && x <= 190) {
            if (y >= 10 && y <= 60) {
                // Load Image
                if (*image != NULL) {
                    MLV_free_image(*image);
                }
                MLV_wait_input_box(200, 200, 400, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, "Enter image filename: ", &filename);
                *image = MLV_load_image(filename);
                if (*image != NULL) {
                    MLV_resize_image_with_proportions(*image, 512, 512);
                    MLV_draw_image(*image, 200, 50);
                    MLV_update_window();
                }
            } else if (y >= 70 && y <= 120) {
                // Quadtree Approximation
                if (*image != NULL) {
                    *tree = create_c_tree_from_image(*image, 0, 0, MLV_get_image_width(*image), MLV_get_image_height(*image));
                    MLV_draw_text(200, 10, "Quadtree created", MLV_COLOR_RED);
                    MLV_update_window();
                }
            } else if (y >= 130 && y <= 180) {
                // Save Binary BW
                if (*tree != NULL) {
                    MLV_wait_input_box(200, 200, 400, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, "Enter filename to save (BW): ", &filename);
                    save_quadtree_binary(*tree, filename);
                    MLV_draw_text(200, 10, "Quadtree saved (BW)", MLV_COLOR_RED);
                    MLV_update_window();
                }
            } else if (y >= 190 && y <= 240) {
                // Save Binary RGBA
                if (*tree != NULL) {
                    MLV_wait_input_box(200, 200, 400, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, "Enter filename to save (RGBA): ", &filename);
                    save_quadtree_binary(*tree, filename);
                    MLV_draw_text(200, 10, "Quadtree saved (RGBA)", MLV_COLOR_RED);
                    MLV_update_window();
                }
            } else if (y >= 250 && y <= 300) {
                // Minimize Quadtree
                if (*tree != NULL) {
                    minimize_quadtree(*tree);
                    MLV_draw_text(200, 10, "Quadtree minimized", MLV_COLOR_RED);
                    MLV_update_window();
                }
            } else if (y >= 310 && y <= 360) {
                // Save Minimized BW
                if (*tree != NULL) {
                    MLV_wait_input_box(200, 200, 400, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, "Enter filename to save (Minimized BW): ", &filename);
                    save_quadtree_binary(*tree, filename);
                    MLV_draw_text(200, 10, "Quadtree saved (Minimized BW)", MLV_COLOR_RED);
                    MLV_update_window();
                }
            } else if (y >= 370 && y <= 420) {
                // Save Minimized RGBA
                if (*tree != NULL) {
                    MLV_wait_input_box(200, 200, 400, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, "Enter filename to save (Minimized RGBA): ", &filename);
                    save_quadtree_binary(*tree, filename);
                    MLV_draw_text(200, 10, "Quadtree saved (Minimized RGBA)", MLV_COLOR_RED);
                    MLV_update_window();
                }
            } else if (y >= 430 && y <= 480) {
                // Open Image
                MLV_wait_input_box(200, 200, 400, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, "Enter filename to open: ", &filename);
                *tree = load_quadtree_binary(filename);
                if (*tree != NULL) {
                    MLV_draw_text(200, 10, "Quadtree loaded", MLV_COLOR_RED);
                    MLV_update_window();
                }
            }
        }
    }
}

color *average_color(MLV_Image *image, int x, int y, int width, int height) {
    int r = 0, g = 0, b = 0, a = 0, count = 0;
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            int vr, vg, vb, va;
            MLV_get_pixel_on_image(image, i, j, &vr, &vg, &vb, &va);
            r += vr;
            g += vg;
            b += vb;
            a += va;
            count++;
        }
    }
    return create_color(r / count, g / count, b / count, a / count);
}

c_node *create_c_tree_from_image(MLV_Image *image, int x, int y, int width, int height) {
    if (width <= 1 && height <= 1) {
        color *c = average_color(image, x, y, width, height);
        return create_c_leaf(c);
    }

    int half_width = width / 2;
    int half_height = height / 2;

    c_node **children = (c_node **)malloc(MAX_CHILDREN * sizeof(c_node *));
    children[0] = create_c_tree_from_image(image, x, y, half_width, half_height);
    children[1] = create_c_tree_from_image(image, x + half_width, y, half_width, half_height);
    children[2] = create_c_tree_from_image(image, x, y + half_height, half_width, half_height);
    children[3] = create_c_tree_from_image(image, x + half_width, y + half_height, half_width, half_height);

    return create_c_node(NULL, children);
}
