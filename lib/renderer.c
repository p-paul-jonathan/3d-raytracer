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
VectorColor compute_lighting(Vector3D sphere_intersection_point,
                             Vector3D sphere_normal_at_intersection_point,
                             Scene scene, float sphere_speculararity,
                             Vector3D view_direction) {

  VectorColor intensity_of_light_at_intersection_point =
      vector_color_init(0, 0, 0, 1);

  Vector3D light_direction_at_sphere;

  for (int i = 0; i < scene.lights_count; i++) {
    Light light = scene.lights[i];
    float multiplier_min = 0.001f, multiplier_max;

    if (light.type == AMBIENT) {
      intensity_of_light_at_intersection_point =
          vector_color_add(intensity_of_light_at_intersection_point,
                           vector_color_scale(light.color, light.intensity));
    } else {

      if (light.type == POINT) {
        light_direction_at_sphere =
            vector_3d_subtract(light.position, sphere_intersection_point);
        multiplier_max = 1.0f;
      } else {
        light_direction_at_sphere = light.direction;
        multiplier_max = INFINITY;
      }

      light_direction_at_sphere =
          vector_3d_unit_vector(light_direction_at_sphere);

      float n_dot_l = vector_3d_dot_product(sphere_normal_at_intersection_point,
                                            light_direction_at_sphere);

      Vector3D shadow_origin =
          vector_3d_add(sphere_intersection_point,
                        vector_3d_multiply_scalar(
                            sphere_normal_at_intersection_point, 0.001f));

      SphereIntersection shadow_intersection = closest_intersection(
          shadow_origin, light_direction_at_sphere, multiplier_min,
          multiplier_max, scene.spheres, scene.spheres_count);

      if (shadow_intersection.hit_sphere)
        continue;

      if (n_dot_l > 0) {
        VectorColor diffuse_contribution =
            vector_color_scale(light.color, light.intensity * n_dot_l);

        intensity_of_light_at_intersection_point = vector_color_add(
            intensity_of_light_at_intersection_point, diffuse_contribution);
      }

      if (sphere_speculararity > -1) {
        Vector3D reflected_ray =
            vector_3d_reflect(vector_3d_negate(light_direction_at_sphere),
                              sphere_normal_at_intersection_point);

        float r_dot_v =
            vector_3d_dot_product(vector_3d_unit_vector(reflected_ray),
                                  vector_3d_unit_vector(view_direction));

        if (r_dot_v > 0) {
          float spec = calculate_sphere_shine(sphere_speculararity, r_dot_v);

          VectorColor specular_contribution =
              vector_color_scale(light.color, light.intensity * spec);

          intensity_of_light_at_intersection_point = vector_color_add(
              intensity_of_light_at_intersection_point, specular_contribution);
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
                      float multiplier_min, float multiplier_max, Scene scene,
                      int recurstion_depth) {

  SphereIntersection intersection =
      closest_intersection(camera_position, ray_from_camera, multiplier_min,
                           multiplier_max, scene.spheres, scene.spheres_count);

  if (!intersection.hit_sphere)
    return BACKGROUND_COLOR;

  if (intersection.closest_sphere.light_source)
    return intersection.closest_sphere.color;

  Vector3D sphere_intersection_point = vector_3d_add(
      camera_position, vector_3d_multiply_scalar(
                           ray_from_camera, intersection.closest_multiplier));
  Vector3D sphere_normal_at_intersection_point =
      vector_3d_unit_vector(vector_3d_subtract(
          sphere_intersection_point, intersection.closest_sphere.center));

  VectorColor intensity = compute_lighting(
      sphere_intersection_point, sphere_normal_at_intersection_point, scene,
      intersection.closest_sphere.specular, vector_3d_negate(ray_from_camera));

  VectorColor local_color =
      vector_color_mul(intersection.closest_sphere.color, intensity);
  float reflectiveness = intersection.closest_sphere.reflectiveness;

  if (recurstion_depth <= 0 || reflectiveness <= 0)
    return local_color;

  Vector3D reflected_ray = vector_3d_unit_vector(
      vector_3d_reflect(ray_from_camera, sphere_normal_at_intersection_point));

  Vector3D offset_origin = vector_3d_add(
      sphere_intersection_point,
      vector_3d_multiply_scalar(sphere_normal_at_intersection_point, 0.001f));

  VectorColor reflected_color =
      trace_ray(offset_origin, reflected_ray, 0.001, INFINITY, scene,
                recurstion_depth - 1);

  VectorColor weighted_color =
      vector_color_add(vector_color_scale(local_color, 1 - reflectiveness),
                       vector_color_scale(reflected_color, reflectiveness));

  return weighted_color;
}

void render_scene(Camera camera, Scene scene, uint32_t *framebuffer,
                  bool render_in_hd) {
  int half_width = WINDOW_WIDTH / 2;
  int half_height = WINDOW_HEIGHT / 2;

  int jump_step = render_in_hd ? 1 : 8;

  for (int i = -half_width; i < half_width; i += jump_step) {
    for (int j = -half_height; j < half_height; j += jump_step) {

      Vector3D ray_from_camera =
          vector_3d_unit_vector(canvas_to_viewport(camera, i, j));

      VectorColor pixel_color =
          trace_ray(camera.position, ray_from_camera, camera.min_range,
                    camera.max_range, scene, MAX_RECURSION_DEPTH);

      for (int w = i; w < i + jump_step; w++) {
        for (int x = j; x < j + jump_step; x++) {
          put_pixel(w, x, vector_color_to_argb8888(pixel_color), framebuffer);
        }
      }
    }
  }
}
