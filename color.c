#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "color.h"

color *create_color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
  color *c = (color *)malloc(sizeof(color));
  c->red = red;
  c->green = green;
  c->blue = blue;
  c->alpha = alpha;
  return c;
}

void print_color(color c) {
  printf("rgb(%d, %d, %d, %d)\n", c.red, c.green, c.blue, c.alpha);
}

float c_c_distance(color c1, color c2) {
  return sqrt(abs(c1.red - c2.red) +
    abs(c1.green - c2.green) +
    abs(c1.blue - c2.blue) + 
    abs(c1.alpha - c2.alpha));
}

float c_distance(color c1) {
  return sqrt(abs(c1.red) +
    abs(c1.green) +
    abs(c1.blue) + 
    abs(c1.alpha));
}

void free_color(color *c) {
  free(c);
}
