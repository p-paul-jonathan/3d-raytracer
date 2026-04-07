#ifndef SPHERE_H
#define SPHERE_H

#include "vector_3d.h"
#include "vector_color.h"

typedef struct {
  Vector3D center;
  float radius;
  VectorColor color;
} Sphere;

typedef struct {
  float multiplier_1;
  float multiplier_2;
} SphereIntersection;

SphereIntersection calculate_sphere_intersection_points(Vector3D ray_origin,
                                                        Vector3D ray_direction,
                                                        Sphere sphere);
#endif // SPHERE_H
