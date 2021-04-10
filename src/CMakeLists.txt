find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(TBB REQUIRED tbb)

add_library(sources
        primitives/color.cc
        primitives/point.cc
        primitives/vector.cc

        scene/camera.cc
        scene/objects/object.cc
        scene/point_light.cc
        scene/objects/sphere.cc
        scene/objects/plane.cc
        scene/objects/triangle.cc
        scene/objects/rectangle.cc
        scene/objects/cube.cc
        scene/textures/uniform_texture.cc
        scene/textures/procedural_texture.cc
        scene/textures/transparent_texture.cc
        scene/textures/image_texture.cc
        scene/textures/perlin_texture.cc
        scene/blob.cc
        scene/textures/bump_mapping_texture.cc

        utils/image.cc
        utils/perlin_noise.cc
        utils/parser.cc

        core/engine.cc
        )

set_target_properties(sources PROPERTIES
        CXX_STANDARD 17
        )

target_link_libraries(sources PRIVATE
        common_options
        )

add_executable(ray-tracer
        main.cc
        )

set_target_properties(ray-tracer PROPERTIES
        CXX_STANDARD 17
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        )

target_link_libraries(ray-tracer PRIVATE
        common_options
        sources
        nlohmann_json::nlohmann_json
        tbb
        )