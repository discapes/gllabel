cmake_minimum_required(VERSION 3.10)
project(GLLabel VERSION 0.1 LANGUAGES CXX)

option(BUILD_DEMO "Build demo executable." ON)
set(FREETYPE_INCLUDE_DIR /usr/include/freetype2 CACHE STRING "Freetype include directory.")
if (NOT EXISTS "${CMAKE_BINARY_DIR}/CMakeCache.txt" AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING 
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
endif()

add_subdirectory(src)

if (BUILD_DEMO)
	add_subdirectory(demo)
endif()
