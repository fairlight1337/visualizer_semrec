cmake_minimum_required(VERSION 2.8.3)
project(visualizer_semrec)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg")

find_package(catkin REQUIRED COMPONENTS
  designator_integration_cpp
  roscpp
  semrec
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES visualizer_semrec
#  CATKIN_DEPENDS designator_integration_cpp roscpp semrec
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
  "3rdparty/SDL2-2.0.3/include"
  "3rdparty/SDL2_image-2.0.0"
  "3rdparty/SDL2_ttf-2.0.12"
  "3rdparty/SDL2_gfx-1.0.1")

link_directories(
  "${PROJECT_SOURCE_DIR}/3rdparty/SDL2-2.0.3/build/.libs"
  "${PROJECT_SOURCE_DIR}/3rdparty/SDL2_image-2.0.0/.libs"
  "${PROJECT_SOURCE_DIR}/3rdparty/SDL2_ttf-2.0.12/.libs"
  "${PROJECT_SOURCE_DIR}/3rdparty/SDL2_gfx-1.0.1/.libs")

add_library(sr_plugin_visualizer_semrec
  src/PluginVisualizer.cpp
  src/TreeNode.cpp
  src/Drawable.cpp
  src/Physics.cpp)

target_link_libraries(sr_plugin_visualizer_semrec
  ${catkin_LIBRARIES}
  sr_base_plugins
  sr_plugin_owlexporter
  SDL2
  SDL2_gfx
  SDL2_image
  SDL2_ttf)
