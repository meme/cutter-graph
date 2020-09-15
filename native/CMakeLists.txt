cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(GraphLayout CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-Oz -g0")

add_executable(GraphLayout GraphLayout.cpp GraphGridLayout.cpp)
set_target_properties(GraphLayout PROPERTIES
  CXX_STANDARD 17
  LINK_FLAGS "--bind"
)