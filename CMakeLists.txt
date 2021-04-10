# SETUP
cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 1.0.0 LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# COMMON OPTIONS
add_library(common_options INTERFACE)
target_compile_options(common_options INTERFACE
        "-Wall"
        "-Werror"
        "-Wextra"
        "-pedantic"
        "-lpthread"
        "-pthread"
        )


target_compile_options(common_options INTERFACE
        "-O2"
        )

target_include_directories(common_options INTERFACE
        src/
        )

# END COMMON OPTIONS

# LIBRARIES

add_subdirectory(src)