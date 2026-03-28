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
  return vector_3d_init((a.y * b.z) - (b.z * a.y), (a.x * b.z) - (b.z * a.x),
                        (a.x * b.y) - (b.y * a.x));
}

Vector3D vector_3d_negate(Vector3D v) {
  return vector_3d_init(-v.x, -v.y, -v.z);
}

float vector_3d_magnitude(Vector3D v) {
  return sqrtf(vector_3d_dot_product(v, v));
}

Vector3D vector_3d_unit_vector(Vector3D v) {
  return vector_3d_multiply_scalar(v, vector_3d_magnitude(v));
}

Vector3D vector_3d_reflect(Vector3D v, Vector3D normal) {
  Vector3D normal_unit_vector = vector_3d_unit_vector(normal);

  return vector_3d_subtract(
      v, vector_3d_multiply_scalar(
             normal_unit_vector,
             2 * vector_3d_dot_product(v, normal_unit_vector)));
}

bool vector_3d_equal(Vector3D a, Vector3D b, float err) {
  return (fabs(a.x - b.x) < err && fabs(a.y - b.y) < err &&
          fabs(a.z - b.z) < err);
}
