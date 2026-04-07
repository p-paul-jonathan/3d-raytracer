#include "sphere.h"
#include "vector_3d.h"
#include <math.h>

SphereIntersection calculate_sphere_intersection_points(Vector3D ray_origin,
                                                        Vector3D ray_direction,
                                                        Sphere sphere) {

  float r = sphere.radius;
  Vector3D CO = vector_3d_subtract(ray_origin, sphere.center);

  float a = vector_3d_dot_product(ray_direction, ray_direction);
  float b = 2 * vector_3d_dot_product(CO, ray_direction);
  float c = vector_3d_dot_product(CO, CO) - (r * r);

  float d = b * b - 4 * a * c;

  if (d < 0) // did not intersect
    return (SphereIntersection){INFINITY, INFINITY};

  float sqrt_d = sqrtf(d);

  float t1 = (-b - sqrt_d) / (2 * a); // near
  float t2 = (-b + sqrt_d) / (2 * a); // far

  return (SphereIntersection){t1, t2};
}
