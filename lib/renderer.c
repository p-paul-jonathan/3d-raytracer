#include "renderer.h"
#include "camera.h"
#include "constants.h"
#include "light.h"
#include "scene.h"
#include "sphere.h"
#include "vector_3d.h"
#include "vector_color.h"
#include <math.h>
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

// Computes the Light intensity on each point in camera
// by weighted cosine sum of all lights falling on it
float compute_lighting(Vector3D sphere_intersection_point,
                       Vector3D sphere_normal_at_intersection_point,
                       Scene scene, float sphere_speculararity,
                       Vector3D view_direction) {
  float intensity_of_light_at_intersection_point = 0.0f;
  Vector3D light_direction_at_sphere;

  for (int i = 0; i < scene.lights_count; i++) {
    Light light = scene.lights[i];
    float multiplier_min = 0.001f, multiplier_max;

    if (light.type == AMBIENT) {
      // we add intensity to every visible point on the sphere
      intensity_of_light_at_intersection_point += light.intensity;
    } else {
      if (light.type == POINT) {
        light_direction_at_sphere =
            vector_3d_subtract(light.position, sphere_intersection_point);
        multiplier_max = 1.0f;
      } else {
        light_direction_at_sphere = light.direction;
        multiplier_max = INFINITY;
      }

      float n_dot_l = vector_3d_dot_product(sphere_normal_at_intersection_point,
                                            light_direction_at_sphere);

      SphereIntersection shadow_intersection = closest_intersection(
          sphere_intersection_point, light_direction_at_sphere, multiplier_min,
          multiplier_max, scene.spheres, scene.spheres_count);

      if (shadow_intersection.hit_sphere)
        continue;

      if (n_dot_l > 0) {
        intensity_of_light_at_intersection_point +=
            n_dot_l /
            (vector_3d_magnitude(sphere_normal_at_intersection_point) *
             vector_3d_magnitude(light_direction_at_sphere));
      }

      if (sphere_speculararity > -1) {
        Vector3D reflected_ray = vector_3d_multiply_scalar(
            sphere_normal_at_intersection_point, 2 * n_dot_l);
        float r_dot_v = vector_3d_dot_product(reflected_ray, view_direction);

        if (r_dot_v > 0) {
          intensity_of_light_at_intersection_point +=
              light.intensity *
              calculate_sphere_shine(reflected_ray, view_direction,
                                     sphere_speculararity, r_dot_v);
        }
      }
    }
  }

  return intensity_of_light_at_intersection_point;
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

  SphereIntersection intersection =
      closest_intersection(camera_position, ray_from_camera, 0, INFINITY,
                           scene.spheres, scene.spheres_count);

  if (!intersection.hit_sphere)
    return vector_color_black();

  Vector3D sphere_intersection_point = vector_3d_add(
      camera_position, vector_3d_multiply_scalar(
                           ray_from_camera, intersection.closest_multiplier));
  Vector3D sphere_normal_at_intersection_point =
      vector_3d_unit_vector(vector_3d_subtract(
          sphere_intersection_point, intersection.closest_sphere.center));

  float intensity = compute_lighting(
      sphere_intersection_point, sphere_normal_at_intersection_point, scene,
      intersection.closest_sphere.specular, vector_3d_negate(ray_from_camera));

  return vector_color_scale(intersection.closest_sphere.color, intensity);
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
