#include <stdio.h>
#include <stdlib.h>
#include "color.h"

color *create_color(int red, int green, int blue) {
  color *c = (color *)malloc(sizeof(color));
  c->red = red > 255 ? 255 : red;
  c->green = green > 255 ? 255 : green;
  c->blue = blue > 255 ? 255 : blue;
  return c;
}

void print_color(color c) {
  printf("rgb(%d, %d, %d)\n", c.red, c.green, c.blue);
}

int distance(color c1, color c2) {
  return sqrt(abs(c1.red - c2.red) + abs(c1.green - c2.green) + abs(c1.blue - c2.blue));
}

void free_color(color *c) {
  free(c);
}
