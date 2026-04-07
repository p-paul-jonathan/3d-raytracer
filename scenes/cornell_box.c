#include "../lib/light.h"
#include "../lib/scene.h"
#include "../lib/sphere.h"
#include "../lib/vector_3d.h"
#include "../lib/vector_color.h"
#include <stdbool.h>
#include <stdlib.h>

Scene *create_cornell_box_scene() {
  int spheres_count = 9;
  Sphere *spheres = malloc(spheres_count * sizeof(Sphere));

  // walls
  spheres[0] = (Sphere){vector_3d_init(-5020, 0, 0),
                        5000,
                        vector_color_init(0.75, 0.15, 0.15, 1),
                        -1,
                        0.0,
                        false};

  spheres[1] = (Sphere){vector_3d_init(5020, 0, 0),
                        5000,
                        vector_color_init(0.15, 0.15, 0.75, 1),
                        -1,
                        0.0,
                        false};

  spheres[2] = (Sphere){vector_3d_init(0, 0, 5050),
                        5000,
                        vector_color_init(0.15, 0.75, 0.15, 1),
                        -1,
                        0.0,
                        false};

  spheres[3] = (Sphere){vector_3d_init(0, -5009, 0),
                        5000,
                        vector_color_init(0.98, 0.94, 0.88, 1),
                        -1,
                        0.05,
                        false};

  spheres[4] = (Sphere){vector_3d_init(0, 5009, 0),
                        5000,
                        vector_color_init(0.98, 0.94, 0.88, 1),
                        -1,
                        0.0,
                        false};

  // objects
  spheres[5] = (Sphere){vector_3d_init(-10, 0, 40),
                        3,
                        vector_color_init(0.7, 0.7, 0.7, 1),
                        500,
                        0.2,
                        false};

  spheres[6] = (Sphere){vector_3d_init(10, 0, 40),
                        3,
                        vector_color_init(0.7, 0.7, 0.7, 1),
                        1500,
                        0.4,
                        false};

  spheres[7] = (Sphere){vector_3d_init(0, 0, 42),
                        3,
                        vector_color_init(0.7, 0.7, 0.7, 1),
                        3000,
                        0.8,
                        false};

  // LIGHT SPHERE (visible)
  spheres[8] = (Sphere){vector_3d_init(0, 8, 40),
                        2,
                        vector_color_init(1.0, 0.95, 0.8, 1), // warm light
                        -1,
                        0.0,
                        true};

  int lights_count = 1;
  Light *lights = malloc(lights_count * sizeof(Light));

  // colored light (matches sphere)
  lights[0] =
      (Light){POINT, 1.2, vector_3d_init(0, 8, 40), vector_3d_init(0, 0, 0),
              vector_color_init(1.0, 0.95, 0.8, 1)};

  Scene *scene = malloc(sizeof(Scene));
  scene->spheres = spheres;
  scene->spheres_count = spheres_count;
  scene->lights = lights;
  scene->lights_count = lights_count;

  return scene;
}
