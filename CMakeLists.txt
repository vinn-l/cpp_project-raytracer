# Specify random library headers and sources
set(material_header include/material.hpp)
set(material_src lib/material.cpp)

# Specify minimum required version
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Specify project and identify languages used.
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add program target called raytrace
add_executable(raytrace app/raytrace.cpp)

# Specify the include directories for executable
target_include_directories(raytrace PUBLIC
 	include
	"${CMAKE_CURRENT_BINARY_DIR}/include")

# Move demo to bin
INSTALL(FILES ${CMAKE_SOURCE_DIR}/demo DESTINATION bin/)

# Install
install(TARGETS raytrace DESTINATION test)