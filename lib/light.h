#ifndef LIGHT_H
#define LIGHT_H

#include "vector_3d.h"
#include "vector_color.h"

typedef enum {
  AMBIENT,     // The Light which is reflected all around the scene
  POINT,       // Light which emenates from a point
  DIRECTIONAL, // Light which has a direction with all parallel rays
} LIGHT_TYPES;

typedef struct {
  LIGHT_TYPES type;
  float intensity;    // all intensity of lights in a scene need to add up to 1
  Vector3D position;  // <0,0,0> for all types except point
  Vector3D direction; // <0,0,0> for all types except directional
  VectorColor color;
} Light;

#endif // LIGHT_H
