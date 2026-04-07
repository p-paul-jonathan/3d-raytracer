cmake_minimum_required(VERSION 3.16)

add_library(3d-raytracer-scenes
  cornell_box.c
  solar_system.c
)

target_include_directories(3d-raytracer-scenes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

