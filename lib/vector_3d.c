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

Vector3D vector_3d_zero() { return vector_3d_init(0, 0, 0); }

Quaternion vector_3d_to_quaternion(Vector3D v) {
  return (Quaternion){0, v.x, v.y, v.z};
}

Vector3D quaternion_to_vector_3d(Quaternion q) {
  return vector_3d_init(q.b, q.c, q.d);
}

Quaternion quaternion_from_axis_angle(Vector3D v, float angle_in_radians) {
  float half_angle = angle_in_radians / 2.0f;
  float sin_angle = sinf(half_angle);
  float cos_angle = cosf(half_angle);

  v = vector_3d_unit_vector(v);

  return (Quaternion){cos_angle, sin_angle * v.x, sin_angle * v.y,
                      sin_angle * v.z};
}

Quaternion quaternion_multiply(Quaternion qa, Quaternion qb) {
  return (Quaternion){
      qa.a * qb.a - qa.b * qb.b - qa.c * qb.c - qa.d * qb.d, // 1
      qa.a * qb.b + qa.b * qb.a + qa.c * qb.d - qa.d * qb.c, // i
      qa.a * qb.c - qa.b * qb.d + qa.c * qb.a + qa.d * qb.b, // j
      qa.a * qb.d + qa.b * qb.c - qa.c * qb.b + qa.d * qb.a  // k
  };
}

Quaternion quaternion_conjugate(Quaternion q) {
  return (Quaternion){q.a, -q.b, -q.c, -q.d};
}

Vector3D rotate_vector_via_quaternion(Vector3D u, Quaternion q) {
  Quaternion q_u = vector_3d_to_quaternion(u);
  Quaternion q_v =
      quaternion_multiply(q, quaternion_multiply(q_u, quaternion_conjugate(q)));

  return quaternion_to_vector_3d(q_v);
}
