#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "scene.h"
#include <stdint.h>

void render_scene(Camera camera, Scene scene, uint32_t *framebuffer, bool render_in_hd);

#endif // RENDERER_H
