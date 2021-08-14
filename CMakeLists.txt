# if ( CMAKE_COMPILER_IS_GNUCC )
#     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -frounding-math")
# endif()
# if ( MSVC )
#     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
# endif()

# set(CMAKE_EXPORT_COMPILE_COMMANDS true)
# set(CMAKE_VERBOSE_MAKEFILE true)

# Specify random library headers and sources
set(material_header include/material.hpp)
set(material_src lib/material.cpp)

# Specify minimum required version
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Specify project and identify languages used.
project(raytracer LANGUAGES CXX)

# TSAN
# option(ENABLE_TSAN "Enable Thread Sanitizer" false)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# if (ENABLE_TSAN)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
# endif()

# set(THREADS_PREFER_PTHREAD_FLAG true)
# find_package(Threads REQUIRED)

# Add program target called test_queue
add_executable(raytrace app/raytrace.cpp)
# add_executable(test_thread_pool app/test_thread_pool.cpp)

# Specify the include directories for library
target_include_directories(raytrace PUBLIC
 	include
	"${CMAKE_CURRENT_BINARY_DIR}/include")

# Add thread pool library
# add_library(material_lib ${material_src})

# Specify the include directories for library
# target_include_directories(material_lib PUBLIC
#  	include
#	"${CMAKE_CURRENT_BINARY_DIR}/include")

# Specify the include directories for library
# target_link_libraries(raytrace material_lib)

# Install
install(TARGETS raytrace DESTINATION test)