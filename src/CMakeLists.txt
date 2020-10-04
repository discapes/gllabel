cmake_minimum_required(VERSION 3.18.2)
include("${CMAKE_SOURCE_DIR}/cmake/CMakeRC.cmake")

add_library(GLLabel
	gridglyph.cpp
	label.cpp
	types.cpp
)
target_compile_definitions(GLLabel PUBLIC _CRT_SECURE_NO_WARNINGS)
target_include_directories(GLLabel INTERFACE include PRIVATE include/gllabel)

cmrc_add_resource_library(Shaders
	glyphshader.vert
	glyphshader.frag
)
find_package(Freetype REQUIRED)
find_package(GLM REQUIRED)
find_package(GLAD REQUIRED)
target_link_libraries(GLLabel Shaders Freetype GLM GLAD)
