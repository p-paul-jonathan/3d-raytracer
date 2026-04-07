#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <stdbool.h>

typedef struct {
  float x;
  float y;
  float z;
} Vector3D;

Vector3D vector_3d_init(float x, float y, float z);
Vector3D vector_3d_add(Vector3D a, Vector3D b);
Vector3D vector_3d_subtract(Vector3D a, Vector3D b);
Vector3D vector_3d_multiply_scalar(Vector3D v, float k);
float vector_3d_dot_product(Vector3D a, Vector3D b);
Vector3D vector_3d_cross_product(Vector3D a, Vector3D b);
Vector3D vector_3d_negate(Vector3D v);
float vector_3d_magnitude(Vector3D v);
Vector3D vector_3d_unit_vector(Vector3D v);

/*
 * This is the reflection of a ray with a normal w.r.t a mirror
 *
 * inc  normal  ref
 * \     ^     ^
 *  \    |    /
 *   \   |   /
 *    \  |  /
 *     V | /
 * ---------------
 *
 *  incident ray: x
 *  normal: n
 *  reflected ray: r
 *
 *  the function normalizes n
 *
 *  <a,b> = a dot b
 *
 *  r = x - 2*(<x, n>)*n
 *
 */
Vector3D vector_3d_reflect(Vector3D v, Vector3D normal);
bool vector_3d_equal(Vector3D a, Vector3D b, float err);

void vector_3d_print(Vector3D v);
Vector3D vector_3d_zero();

#endif // VECTOR_3D_H
