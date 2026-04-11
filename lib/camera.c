#include "camera.h"
#include "constants.h"
#include "vector_3d.h"

Vector3D canvas_to_viewport(Camera camera, float canvas_x, float canvas_y) {
  float vx =
      (canvas_x - WINDOW_WIDTH / 2.0f) * camera.viewport_width / WINDOW_WIDTH;

  float vy = (WINDOW_HEIGHT / 2.0f - canvas_y) * camera.viewport_height /
             WINDOW_HEIGHT;

  return vector_3d_add(
      vector_3d_multiply_scalar(camera.camera_forward,
                                camera.viewport_distance),
      vector_3d_add(vector_3d_multiply_scalar(camera.camera_right, vx),
                    vector_3d_multiply_scalar(camera.camera_up, vy)));
}

void normalize_camera(Camera *camera) {
  camera->camera_forward = vector_3d_unit_vector(camera->camera_forward);
  camera->camera_right = vector_3d_unit_vector(camera->camera_right);
  camera->camera_up = vector_3d_unit_vector(camera->camera_up);
}

void move_camera_up(Camera *camera, float distance) {
  camera->position = vector_3d_add(
      camera->position, vector_3d_multiply_scalar(camera->camera_up, distance));
}
void move_camera_down(Camera *camera, float distance) {
  move_camera_up(camera, -distance);
}

void move_camera_right(Camera *camera, float distance) {
  camera->position =
      vector_3d_add(camera->position,
                    vector_3d_multiply_scalar(camera->camera_right, distance));
}
void move_camera_left(Camera *camera, float distance) {
  move_camera_right(camera, -distance);
}

void move_camera_forward(Camera *camera, float distance) {
  camera->position = vector_3d_add(
      camera->position,
      vector_3d_multiply_scalar(camera->camera_forward, distance));
}
void move_camera_backward(Camera *camera, float distance) {
  move_camera_forward(camera, -distance);
}

void pitch_camera_up(Camera *camera, float angle) {
  Quaternion q = quaternion_from_axis_angle(camera->camera_right, angle);

  camera->camera_forward =
      rotate_vector_via_quaternion(camera->camera_forward, q);
  camera->camera_up = rotate_vector_via_quaternion(camera->camera_up, q);

  normalize_camera(camera);
}
void pitch_camera_down(Camera *camera, float angle) {
  pitch_camera_up(camera, -angle);
}

void yaw_camera_left(Camera *camera, float angle) {
  Quaternion q = quaternion_from_axis_angle(camera->camera_up, angle);

  camera->camera_forward =
      rotate_vector_via_quaternion(camera->camera_forward, q);
  camera->camera_right = rotate_vector_via_quaternion(camera->camera_right, q);

  normalize_camera(camera);
}
void yaw_camera_right(Camera *camera, float angle) {
  yaw_camera_left(camera, -angle);
}

void roll_camera_left(Camera *camera, float angle) {
  Quaternion q = quaternion_from_axis_angle(camera->camera_forward, angle);

  camera->camera_up = rotate_vector_via_quaternion(camera->camera_up, q);
  camera->camera_right = rotate_vector_via_quaternion(camera->camera_right, q);

  normalize_camera(camera);
}
void roll_camera_right(Camera *camera, float angle) {
  roll_camera_left(camera, -angle);
}
