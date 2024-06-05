#ifndef COLOR_H
#define COLOR_H


typedef struct _color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} color;

/* print the color c */
void print_color(color c);

/* measure distance between two colors */
float c_c_distance(color c1, color c2);

/* Returns a distance of a signle color from the origin. */
float c_distance(color c1);

/* create a color from the rgb components */
color *create_color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

/* free the memory allocated for the color c */
void free_color(color *c);

#endif /* COLOR_H */