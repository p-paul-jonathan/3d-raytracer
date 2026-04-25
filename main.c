#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_timer.h"
#include "camera.h"
#include "scene.h"
#include "vector_3d.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include "lib/constants.h"
#include "lib/renderer.h"

#include "scenes/cornell_box.c"
#include "scenes/solar_system.c"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static uint32_t *framebuffer = NULL;

static Scene *scene = NULL;
static Camera *camera = NULL;

static bool render_in_hd = true;
static bool scene_rendered = false;
static bool camera_moved = false;
static const bool *key_states = NULL;

unsigned int last_time = 0, current_time, last_moved_time = 0;

// Use any of these scenes
// 1. Cornell Box
// 2. Solar System
static void initialize_scene() {
  scene = create_cornell_box_scene();
  // scene = create_solar_system_scene();
}

static void initialize_camera() {
  camera = malloc(sizeof(Camera));

  camera->position = vector_3d_init(0, 0, 0);

  // Camera faces positive Z
  camera->camera_forward = vector_3d_init(0, 0, 1);

  // Camera right is positive X
  camera->camera_right = vector_3d_init(1, 0, 0);

  // Camera up is positive Y
  camera->camera_up = vector_3d_init(0, 1, 0);

  camera->viewport_distance = VIEWPORT_DISTANCE;
  camera->viewport_width = VIEWPORT_WIDTH;
  camera->viewport_height = VIEWPORT_HEIGHT;

  camera->min_range = 0;
  camera->max_range = INFINITY;
}

static bool move_camera(float delta_time) {
  float angle_step = 0.1f;
  float distance_step = 5.0f;

  bool moved = false;

  if (key_states[SDL_SCANCODE_UP]) {
    pitch_camera_up(camera, angle_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_DOWN]) {
    pitch_camera_down(camera, angle_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_LEFT]) {
    yaw_camera_left(camera, angle_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_RIGHT]) {
    yaw_camera_right(camera, angle_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_W]) {
    move_camera_forward(camera, distance_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_S]) {
    move_camera_backward(camera, distance_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_A]) {
    move_camera_left(camera, distance_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_D]) {
    move_camera_right(camera, distance_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_Q]) {
    roll_camera_left(camera, angle_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_E]) {
    roll_camera_right(camera, angle_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_R]) {
    move_camera_up(camera, distance_step * delta_time);
    moved = true;
  }
  if (key_states[SDL_SCANCODE_F]) {
    move_camera_down(camera, distance_step * delta_time);
    moved = true;
  }

  return moved;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("3D Raytracer", WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Using textures as they are more memory efficient than simply writing data
  // one pixel at a time Store the pixel data as a texture so it can be
  // rewritten multiple times
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH,
                              WINDOW_HEIGHT);
  if (texture == NULL) {
    SDL_Log("Couldn't create texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // framebuffer to render pixel data to the texture each frame
  framebuffer = malloc(WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t));
  if (framebuffer == NULL) {
    SDL_Log("Couldn't create framebuffer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  initialize_scene();
  if (scene == NULL) {
    SDL_Log("Couldn't create scene");
  }

  initialize_camera();
  if (camera == NULL) {
    SDL_Log("Couldn't create camera");
  }

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {

  current_time = SDL_GetTicks();

  key_states = SDL_GetKeyboardState(NULL);
  bool moved = move_camera((float)(current_time - last_time) / 1000);

  last_time = current_time;

  if (moved) {
    camera_moved = true;
    render_in_hd = false;
    scene_rendered = false;
    last_moved_time = current_time;
  }

  if (!moved && camera_moved && current_time - last_moved_time >= 200 &&
      !scene_rendered) {
    render_in_hd = true;
    scene_rendered = true;
    camera_moved = false;
  }

  render_scene(*camera, *scene, framebuffer, render_in_hd);

  SDL_UpdateTexture(texture, NULL, framebuffer,
                    WINDOW_WIDTH * sizeof(uint32_t));
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (framebuffer) {
    free(framebuffer);
  }
  if (texture) {
    SDL_DestroyTexture(texture);
  }
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }
  if (scene) {
    if (scene->spheres) {
      free(scene->spheres);
      free(scene->lights);
    }

    free(scene);
  }
  if (camera) {
    free(camera);
  }
}
