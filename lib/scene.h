#ifndef SCENE_H
#define SCENE_H

#include "light.h"
#include "sphere.h"

typedef struct {
  Sphere *spheres;
  int spheres_count;
  Light *lights;
  int lights_count;
} Scene;

#endif // SCENE_H
