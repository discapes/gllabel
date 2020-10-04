cmake_minimum_required(VERSION 3.10)

project(GLLabel VERSION 0.1 LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(src)

option(BUILD_DEMO "Build demo executable" ON)
if (BUILD_DEMO)
	add_subdirectory(demo)
endif()
