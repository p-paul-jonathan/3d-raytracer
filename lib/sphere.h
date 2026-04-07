#ifndef SPHERE_H
#define SPHERE_H

#include "vector_3d.h"
#include "vector_color.h"

typedef struct {
  Vector3D center;
  float radius;
  VectorColor color;
  float specular; // higher the value, the more shiny the sphere will be
} Sphere;

typedef struct {
  float multiplier_1;
  float multiplier_2;
} SphereIntersectionPoints;

typedef struct {
  bool hit_sphere;
  Sphere closest_sphere;
  float closest_multiplier;
} SphereIntersection;

float calculate_sphere_shine(Vector3D reflected_ray, Vector3D view_direction,
                             float sphere_speculararity, float r_dot_v);

SphereIntersection closest_intersection(Vector3D origin, Vector3D ray_direction,
                                        float multiplier_min,
                                        float multiplier_max, Sphere *spheres,
                                        int spheres_count);

#endif // SPHERE_H
