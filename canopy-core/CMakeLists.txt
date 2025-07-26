cmake_minimum_required(VERSION 3.15...4.0)

project(
  CanopyEngine
  VERSION 1.0
  LANGUAGES CXX)

# Make sure the root directories are used as include roots.
target_include_directories(libcanopy PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(libcanopy PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(libcanopy STATIC
    src/canopy_engine.cc
    src/input/input.cc
    src/renderer/renderer.cc
    src/state/game_state.cc
    src/util/utils.cc
)
