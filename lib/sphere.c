#include "sphere.h"
#include <math.h>

SphereIntersectionPoints
calculate_sphere_intersection_points(Vector3D ray_origin,
                                     Vector3D ray_direction, Sphere sphere) {

  float r = sphere.radius;
  Vector3D CO = vector_3d_subtract(ray_origin, sphere.center);

  float a = vector_3d_dot_product(ray_direction, ray_direction);
  float b = 2 * vector_3d_dot_product(CO, ray_direction);
  float c = vector_3d_dot_product(CO, CO) - (r * r);

  float d = b * b - 4 * a * c;

  if (d < 0) // did not intersect
    return (SphereIntersectionPoints){INFINITY, INFINITY};

  float sqrt_d = sqrtf(d);

  float t1 = (-b - sqrt_d) / (2 * a); // near
  float t2 = (-b + sqrt_d) / (2 * a); // far

  return (SphereIntersectionPoints){t1, t2};
}

float calculate_sphere_shine(Vector3D reflected_ray, Vector3D view_direction,
                             float sphere_speculararity, float r_dot_v) {
  return powf(r_dot_v / (vector_3d_magnitude(reflected_ray) *
                         vector_3d_magnitude(view_direction)),
              sphere_speculararity);
}

// calculate the closest sphere in the scene from the source
// used to calculate main render and shadows
SphereIntersection closest_intersection(Vector3D origin, Vector3D ray_direction,
                                        float multiplier_min,
                                        float multiplier_max, Sphere *spheres,
                                        int spheres_count) {
  bool hit_sphere = false;
  Sphere closest_sphere;
  float closest_multiplier = multiplier_max;

  for (int i = 0; i < spheres_count; i++) {
    SphereIntersectionPoints intersection =
        calculate_sphere_intersection_points(origin, ray_direction, spheres[i]);

    if (intersection.multiplier_1 >= multiplier_min &&
        intersection.multiplier_1 <= multiplier_max &&
        intersection.multiplier_1 < closest_multiplier) {
      closest_sphere = spheres[i];
      closest_multiplier = intersection.multiplier_1;
      hit_sphere = true;
    }

    if (intersection.multiplier_2 >= multiplier_min &&
        intersection.multiplier_2 <= multiplier_max &&
        intersection.multiplier_2 < closest_multiplier) {
      closest_sphere = spheres[i];
      closest_multiplier = intersection.multiplier_2;
      hit_sphere = true;
    }
  }

  return (SphereIntersection){hit_sphere, closest_sphere, closest_multiplier};
}
