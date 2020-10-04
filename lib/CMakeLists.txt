cmake_minimum_required(VERSION 3.18.2)
include("${CMAKE_SOURCE_DIR}/cmake/CMakeRC.cmake")

option(BUILD_SHARED_LIBS "Build shared libraries." ON)
add_library(GLLabel
	vgrid.cpp
	label.cpp
	types.cpp
)

target_compile_definitions(GLLabel PUBLIC _CRT_SECURE_NO_WARNINGS)
target_include_directories(GLLabel INTERFACE include PRIVATE include/gllabel PUBLIC "${FREETYPE_INCLUDE_DIR}")

cmrc_add_resource_library(shaders
	glyphshader.vert
	glyphshader.frag
)
target_link_libraries(GLLabel shaders freetype glad)
