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

# Check if output.ppm exists, delete if so
if (EXISTS ${CMAKE_SOURCE_DIR}/output.ppm)
	file(REMOVE ${CMAKE_SOURCE_DIR}/output.ppm)
endif()

# Create script
file(
	WRITE ${CMAKE_INSTALL_PREFIX}/tmp/demo
	"#!/usr/bin/env bash
	${CMAKE_INSTALL_PREFIX}/test/raytrace < ${CMAKE_SOURCE_DIR}/input.txt >> ${CMAKE_SOURCE_DIR}/output.ppm
	display output.ppm
	")

# Add execute permissions
file(
    COPY ${CMAKE_INSTALL_PREFIX}/tmp/demo
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Remove old file
file(REMOVE_RECURSE ${CMAKE_INSTALL_PREFIX}/tmp/)

# Install
install(TARGETS raytrace DESTINATION test)