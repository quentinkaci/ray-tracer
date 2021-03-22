add_library(sources
        primitives/color.cc
        primitives/point.cc
        primitives/vector.cc

        scene/camera.cc
        scene/object.cc
        scene/point_light.cc
        scene/scene.cc
        scene/sphere.cc
        scene/plane.cc
        scene/triangle.cc
        scene/rectangle.cc
        scene/cube.cc
        scene/uniform_texture.cc
        scene/procedural_texture.cc
        scene/image_texture.cc
        scene/perlin_texture.cc
        scene/blob.cc
        scene/bump_mapping_texture.cc

        utils/image.cc
        utils/perlin_noise.cc

        engine.cc
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
)