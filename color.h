#ifndef COLOR_H
#define COLOR_H

typedef struct _color {
  int red;
  int green;
  int blue;
} color;

/* print the color c */
void print_color(color c);

/* measure distance between two colors */
int distance(color c1, color c2);

/* create a color from the rgb components */
color *create_color(int red, int green, int blue);

/* free the memory allocated for the color c */
void free_color(color *c);

#endif /* COLOR_H */