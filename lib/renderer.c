#include "renderer.h"
#include "constants.h"


// function to color a pixel on the screen using a co-ordinate system
// with origin at center and positives at top and right and negatives at bottom and left
void put_pixel(int x, int y, uint32_t color, uint32_t *framebuffer) {
  int half_width = WINDOW_WIDTH / 2;
  int half_height = WINDOW_HEIGHT / 2;

  if (x < -half_width || x > half_width) {
    return;
  }
  if (y < -half_height || y > half_height) {
    return;
  }

  int screen_x = half_width + x;
  int screen_y = half_height - y;

  framebuffer[screen_x + WINDOW_WIDTH * screen_y] = color;
}

