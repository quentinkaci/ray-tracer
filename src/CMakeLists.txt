add_library(sources
        primitives/color.cc
        primitives/point.cc
        primitives/vector.cc
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