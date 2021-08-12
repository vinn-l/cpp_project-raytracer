# if ( CMAKE_COMPILER_IS_GNUCC )
#     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -frounding-math")
# endif()
# if ( MSVC )
#     set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
# endif()

# set(CMAKE_EXPORT_COMPILE_COMMANDS true)
# set(CMAKE_VERBOSE_MAKEFILE true)

# Specify random library headers and sources
# set(thread_pool_header include/ra/thread_pool.hpp)
# set(thread_pool_sources lib/thread_pool.cpp)

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

# Add thread pool library
# add_library(thread_pool_lib ${thread_pool_sources} ${thread_pool_header})

# Specify the include directories for library
# target_link_libraries(test_queue PUBLIC Threads::Threads)

# Specify the include directories for library
# target_link_libraries(thread_pool_lib PUBLIC Threads::Threads)
# target_link_libraries(test_thread_pool thread_pool_lib)

# Specify the include directories for library
# target_include_directories(test_queue PUBLIC
# 	include
# 	"${CMAKE_CURRENT_BINARY_DIR}/include")

# Specify the include directories for library
# target_include_directories(test_thread_pool PUBLIC
# 	include
# 	"${CMAKE_CURRENT_BINARY_DIR}/include")

# Specify the include directories for library
# target_include_directories(thread_pool_lib PUBLIC
# 	include
# 	"${CMAKE_CURRENT_BINARY_DIR}/include")

# Install
install(TARGETS raytrace DESTINATION test)
# install(TARGETS test_thread_pool DESTINATION test)