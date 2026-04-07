#include "../lib/scene.h"
#include <stdlib.h>

Scene *create_solar_system_scene() {
  int spheres_count = 5;
  Sphere *spheres = malloc(spheres_count * sizeof(Sphere));

  // ☀️ Sun (light source)
  spheres[0] = (Sphere){vector_3d_init(0, 0, 100),
                        10,
                        vector_color_init(1.0, 0.95, 0.8, 1),
                        -1,
                        0.0,
                        true};

  // 🌍 Earth
  spheres[1] = (Sphere){vector_3d_init(30, 0, 100),
                        3,
                        vector_color_init(0.2, 0.4, 1.0, 1),
                        200,
                        0.2,
                        false};

  // 🌙 Moon
  spheres[2] = (Sphere){vector_3d_init(35, 0, 100),
                        1,
                        vector_color_init(0.7, 0.7, 0.7, 1),
                        50,
                        0.1,
                        false};

  // 🔴 Mars
  spheres[3] = (Sphere){vector_3d_init(-50, 0, 120),
                        2,
                        vector_color_init(0.8, 0.3, 0.2, 1),
                        100,
                        0.2,
                        false};

  // 🟡 Venus
  spheres[4] = (Sphere){vector_3d_init(-25, 0, 90),
                        2.5,
                        vector_color_init(0.9, 0.8, 0.6, 1),
                        100,
                        0.1,
                        false};

  int lights_count = 1;
  Light *lights = malloc(lights_count * sizeof(Light));

  // sunlight (warm)
  lights[0] =
      (Light){POINT, 1.5, vector_3d_init(0, 0, 100), vector_3d_init(0, 0, 0),
              vector_color_init(1.0, 0.95, 0.8, 1)};

  Scene *scene = malloc(sizeof(Scene));
  scene->spheres = spheres;
  scene->spheres_count = spheres_count;
  scene->lights = lights;
  scene->lights_count = lights_count;

  return scene;
}
