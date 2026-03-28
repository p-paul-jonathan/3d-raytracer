#ifndef CAMERA_H
#define CAMERA_H

#include "vector_3d.h"

typedef struct {
  Vector3D position;
  Vector3D camera_up;
  Vector3D camera_right;
  Vector3D camera_forward;
  float viewport_width;
  float viewport_height;
  float viewport_distance;
} Camera;

#endif // CAMERA_H
