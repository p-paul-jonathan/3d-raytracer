#ifndef CAMERA_H
#define CAMERA_H

#include "vector_3d.h"

#define PITCH_RANGE MATH_PI / 2
#define YAW_RANGE MATH_PI / 2
#define ROLL_RANGE MATH_PI / 4

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

void move_camera_up(Camera *camera, float distance);
void move_camera_down(Camera *camera, float distance);
void move_camera_left(Camera *camera, float distance);
void move_camera_right(Camera *camera, float distance);
void move_camera_forward(Camera *camera, float distance);
void move_camera_backward(Camera *camera, float distance);

void pitch_camera_up(Camera *camera, float angle);
void pitch_camera_down(Camera *camera, float angle);

void yaw_camera_left(Camera *camera, float angle);
void yaw_camera_right(Camera *camera, float angle);

void roll_camera_left(Camera *camera, float angle);
void roll_camera_right(Camera *camera, float angle);

#endif // CAMERA_H
