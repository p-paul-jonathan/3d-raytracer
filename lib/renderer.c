#include "renderer.h"
#include "camera.h"
#include "constants.h"
#include "scene.h"
#include "sphere.h"
#include "vector_3d.h"
#include "vector_color.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// function to color a pixel on the screen using a co-ordinate system
// with origin at center and positives at top and right and negatives at bottom
// and left
void put_pixel(int x, int y, uint32_t color, uint32_t *framebuffer) {
  int half_width = WINDOW_WIDTH / 2;
  int half_height = WINDOW_HEIGHT / 2;

  if (x < -half_width || x >= half_width)
    return;
  if (y < -half_height || y >= half_height)
    return;

  int screen_x = half_width + x;
  int screen_y = half_height - y - 1;

  framebuffer[screen_x + WINDOW_WIDTH * screen_y] = color;
}

// Check whether the ray intersects anything on the scene
// gives the color of the object
// else gives black
//
// multiplier is the multiplier of the ray as a unit vector which reaches a
// particulr object - ranges from 0 to infinity - can be defined as the range of
// the camera
VectorColor trace_ray(Vector3D camera_position, Vector3D ray_from_camera,
                      float multiplier_min, float multiplier_max, Scene scene) {
  bool hit_sphere = false;
  Sphere closest_sphere;
  float closest_multiplier = multiplier_max;

  for (int i = 0; i < scene.spheres_count; i++) {
    SphereIntersection intersection = calculate_sphere_intersection_points(
        camera_position, ray_from_camera, scene.spheres[i]);

    if (intersection.multiplier_1 >= multiplier_min &&
        intersection.multiplier_1 <= multiplier_max &&
        intersection.multiplier_1 < closest_multiplier) {
      closest_sphere = scene.spheres[i];
      closest_multiplier = intersection.multiplier_1;
      hit_sphere = true;
    }

    if (intersection.multiplier_2 >= multiplier_min &&
        intersection.multiplier_2 <= multiplier_max &&
        intersection.multiplier_2 < closest_multiplier) {
      closest_sphere = scene.spheres[i];
      closest_multiplier = intersection.multiplier_2;
      hit_sphere = true;
    }
  }

  if (!hit_sphere)
    return vector_color_black();

  return closest_sphere.color;
}

void render_scene(Camera camera, Scene scene, uint32_t *framebuffer) {
  int half_width = WINDOW_WIDTH / 2;
  int half_height = WINDOW_HEIGHT / 2;

  for (int i = -half_width; i < half_width; i++) {
    for (int j = -half_height; j < half_height; j++) {

      Vector3D ray_from_camera =
          vector_3d_unit_vector(canvas_to_viewport(camera, i, j));

      VectorColor pixel_color =
          trace_ray(camera.position, ray_from_camera, camera.min_range,
                    camera.max_range, scene);

      put_pixel(i, j, vector_color_to_argb8888(pixel_color), framebuffer);
    }
  }
}
