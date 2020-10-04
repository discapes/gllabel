cmake_minimum_required(VERSION 3.18.2)
include("${CMAKE_SOURCE_DIR}/cmake/CMakeRC.cmake")

project(Demo VERSION 0.01 LANGUAGES CXX)
add_executable(Demo demo.cpp)
target_compile_definitions(Demo PRIVATE _CRT_SECURE_NO_WARNINGS)

cmrc_add_resource_library(Fonts LiberationSans-Bold.ttf LiberationSans-Regular.ttf)
find_package(GLM)
find_package(GLAD)
find_package(GLFW)
find_package(Freetype)
target_link_libraries(Demo Fonts GLM GLAD GLFW Freetype GLLabel)
