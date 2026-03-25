/*
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "lib/constants.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static uint32_t *framebuffer = NULL;

int position = 1;

void put_pixel(int x, int y, uint32_t color, uint32_t *framebuffer) {
  int half_width = WINDOW_WIDTH / 2;
  int half_height = WINDOW_HEIGHT / 2;

  if (x < -half_width || x > half_width)  { return; }
  if (y < -half_height || y > half_height) { return; }

  int screen_x = half_width + x;
  int screen_y = half_height - y;

  framebuffer[screen_x + WINDOW_WIDTH * screen_y] = color;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("3D Raytracer", WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Store the pixel data as a texture so it can be rewritten multiple times
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
  position += 1;
  put_pixel(position, 0, 0xFF0000FF, framebuffer); // blue, right
  put_pixel(0, position, 0xFFFF0000, framebuffer); // red, top
  put_pixel(-position, 0, 0xFF00FF00, framebuffer); // green, left
  put_pixel(0, -position, 0xFFFFFFFF, framebuffer); // white, bottom

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
}
