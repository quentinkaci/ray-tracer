cmake_minimum_required(VERSION 3.17)
project(raytracer LANGUAGES CXX)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(TBB REQUIRED tbb)

# ---
# Set paths to sources
# ---

include_directories(src)

set(SRC
    src/primitives/color.cc
    src/primitives/point.cc
    src/primitives/vector.cc

    src/scene/camera.cc
    src/scene/objects/object.cc
    src/scene/point_light.cc
    src/scene/objects/sphere.cc
    src/scene/objects/plane.cc
    src/scene/objects/triangle.cc
    src/scene/objects/rectangle.cc
    src/scene/objects/cube.cc
    src/scene/textures/uniform_texture.cc
    src/scene/textures/procedural_texture.cc
    src/scene/textures/transparent_texture.cc
    src/scene/textures/image_texture.cc
    src/scene/textures/perlin_texture.cc
    src/scene/blob.cc
    src/scene/textures/bump_mapping_texture.cc

    src/utils/image.cc
    src/utils/perlin_noise.cc
    src/utils/parser.cc

    src/core/engine.cc
)

# ---
# Set compilation flags and C++ standard
# ---

set(OPT_FLAGS "-Ofast -march=native -flto")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

# ---
# Detect build type (release or debug)
# ---

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")

    string(REGEX REPLACE ".*/build_" "" BUILD_DIR_NAME ${CMAKE_BINARY_DIR})
    if(${BUILD_DIR_NAME} STREQUAL "debug")
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "---")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "---")

# ---
# Create target `raytracer`
# ---

add_executable(raytracer src/main.cc)

target_sources(raytracer PRIVATE ${SRC})

target_link_libraries(raytracer PRIVATE
    nlohmann_json::nlohmann_json
    tbb
)