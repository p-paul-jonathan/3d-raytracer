cmake_minimum_required(VERSION 3.16)

add_library(3d-raytracer-lib
  renderer.c
  vector_3d.c
  camera.c
)

target_include_directories(3d-raytracer-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
