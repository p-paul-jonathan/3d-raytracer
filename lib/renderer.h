#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

// function to color a pixel on the screen using a co-ordinate system
// with origin at center and positives at top and right and negatives at bottom and left
void put_pixel(int x, int y, uint32_t color, uint32_t *framebuffer);

#endif // RENDERER_H
