#include <math.h>
#include <stdio.h>

#include "vector_color.h"

static float clamp(float x, float min, float max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

VectorColor vector_color_init(float red, float green, float blue, float alpha) {
  return (VectorColor){red, green, blue, alpha};
}

VectorColor vector_color_add(VectorColor a, VectorColor b) {
  return vector_color_init(a.red + b.red, a.green + b.green, a.blue + b.blue,
                           a.alpha + b.alpha);
}

VectorColor vector_color_scale(VectorColor v, float s) {
  return vector_color_init(v.red * s, v.green * s, v.blue * s, v.alpha * s);
}

VectorColor vector_color_mul(VectorColor a, VectorColor b) {
  return vector_color_init(a.red * b.red, a.green * b.green, a.blue * b.blue,
                           a.alpha * b.alpha);
}

bool vector_color_equal(VectorColor a, VectorColor b, float err) {
  return (fabsf(a.red - b.red) < err && fabsf(a.green - b.green) < err &&
          fabsf(a.blue - b.blue) < err && fabsf(a.alpha - b.alpha) < err);
}

uint32_t vector_color_to_argb8888(VectorColor v) {
  uint8_t a = (uint8_t)(clamp(v.alpha, 0.0f, 1.0f) * 255.0f);
  uint8_t r = (uint8_t)(clamp(v.red, 0.0f, 1.0f) * 255.0f);
  uint8_t g = (uint8_t)(clamp(v.green, 0.0f, 1.0f) * 255.0f);
  uint8_t b = (uint8_t)(clamp(v.blue, 0.0f, 1.0f) * 255.0f);

  return (a << 24) | (r << 16) | (g << 8) | b;
}

VectorColor vector_color_red() { return vector_color_init(1, 0, 0, 1); }
VectorColor vector_color_green() { return vector_color_init(0, 1, 0, 1); }
VectorColor vector_color_blue() { return vector_color_init(0, 0, 1, 1); }
VectorColor vector_color_cyan() { return vector_color_init(0, 1, 1, 1); }
VectorColor vector_color_magenta() { return vector_color_init(1, 0, 1, 1); }
VectorColor vector_color_yellow() { return vector_color_init(1, 1, 0, 1); }
VectorColor vector_color_black() { return vector_color_init(0, 0, 0, 1); }
VectorColor vector_color_white() { return vector_color_init(1, 1, 1, 1); }

void vector_color_print(VectorColor v) {
  printf("VectorColor <%f, %f, %f, %f> <RGBA>\n", v.red, v.green, v.blue,
         v.alpha);
}
