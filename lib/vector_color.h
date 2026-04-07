#ifndef VECTOR_COLOR_H
#define VECTOR_COLOR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  float red;
  float green;
  float blue;
  float alpha;
} VectorColor;

VectorColor vector_color_init(float red, float green, float blue, float alpha);
VectorColor vector_color_add(VectorColor a, VectorColor b);
VectorColor vector_color_scale(VectorColor v, float s);
VectorColor vector_color_mul(VectorColor a, VectorColor b);
bool vector_color_equal(VectorColor a, VectorColor b, float err);
uint32_t vector_color_to_argb8888(VectorColor v);

VectorColor vector_color_red();
VectorColor vector_color_green();
VectorColor vector_color_blue();
VectorColor vector_color_cyan();
VectorColor vector_color_magenta();
VectorColor vector_color_yellow();
VectorColor vector_color_black();
VectorColor vector_color_white();

void vector_color_print(VectorColor v);

#endif // VECTOR_COLOR_H
