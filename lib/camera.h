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
  float min_range;
  float max_range;
} Camera;

// This gives the ray from camera towards scene
// we iterate this over each pixel on the window
// the ray is defined w.r.t scene not camera
Vector3D canvas_to_viewport(Camera camera, float canvas_x, float canvas_y);

#endif // CAMERA_H
