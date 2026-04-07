#include <math.h>
#include <stdio.h>

#include "vector_3d.h"

Vector3D vector_3d_init(float x, float y, float z) {
  return (Vector3D){x, y, z};
}

Vector3D vector_3d_add(Vector3D a, Vector3D b) {
  return vector_3d_init(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3D vector_3d_subtract(Vector3D a, Vector3D b) {
  return vector_3d_init(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3D vector_3d_multiply_scalar(Vector3D v, float k) {
  return vector_3d_init(k * v.x, k * v.y, k * v.z);
}

float vector_3d_dot_product(Vector3D a, Vector3D b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3D vector_3d_cross_product(Vector3D a, Vector3D b) {
  return vector_3d_init(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);
}

Vector3D vector_3d_negate(Vector3D v) {
  return vector_3d_init(-v.x, -v.y, -v.z);
}

float vector_3d_magnitude(Vector3D v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D vector_3d_unit_vector(Vector3D v) {
  float mag = vector_3d_magnitude(v);
  if (mag == 0.0f) {
    return vector_3d_init(0, 0, 0);
  }
  return vector_3d_multiply_scalar(v, 1.0f / mag);
}

Vector3D vector_3d_reflect(Vector3D v, Vector3D normal) {
  Vector3D normal_unit_vector = vector_3d_unit_vector(normal);

  return vector_3d_subtract(
      v, vector_3d_multiply_scalar(
             normal_unit_vector,
             2 * vector_3d_dot_product(v, normal_unit_vector)));
}

bool vector_3d_equal(Vector3D a, Vector3D b, float err) {
  return (fabsf(a.x - b.x) < err && fabsf(a.y - b.y) < err &&
          fabsf(a.z - b.z) < err);
}

void vector_3d_print(Vector3D v) {
  printf("Vector3D <%f, %f, %f>\n", v.x, v.y, v.z);
}

Vector3D vector_3d_zero() {
  return vector_3d_init(0, 0, 0);
}
