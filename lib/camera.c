#include "camera.h"
#include "constants.h"
#include "vector_3d.h"

Vector3D canvas_to_viewport(Camera camera, float canvas_x, float canvas_y) {
  return vector_3d_init(
      (canvas_x * camera.viewport_width) / (float)WINDOW_WIDTH,
      (canvas_y * camera.viewport_height) / (float)WINDOW_HEIGHT,
      camera.viewport_distance);
}
