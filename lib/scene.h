#ifndef SCENE_H
#define SCENE_H

#include "sphere.h"

typedef struct {
  Sphere *spheres;
  int spheres_count;
} Scene;

#endif // SCENE_H
