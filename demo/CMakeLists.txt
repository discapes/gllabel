cmake_minimum_required(VERSION 3.18.2)
include("${CMAKE_SOURCE_DIR}/cmake/CMakeRC.cmake")

project(Demo VERSION 0.01 LANGUAGES CXX)
add_executable(Demo demo.cpp)
target_compile_definitions(Demo PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(Demo PRIVATE "${FREETYPE_INCLUDE_DIR}")

cmrc_add_resource_library(fonts LiberationSans-Bold.ttf LiberationSans-Regular.ttf)
target_link_libraries(Demo fonts glad glfw freetype GLLabel)
