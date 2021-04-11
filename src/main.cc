#include "core/engine.hh"
#include "scene/scene.hh"
#include "utils/parser.hh"

#include "utils/image.hh"

using namespace scene;
using namespace core;
using namespace utils;

int main(int argc, char* argv[])
{
    Scene   scene;
    Options options;

    if (argc > 1)
        parse_json(argv[1], options, scene);

    auto image = create_image(options.rendering_output_filename);
    image->create(options.rendering_width, options.rendering_height);

    Engine engine(options, scene);
    engine.run(*image);

    image->save(options.rendering_output_filename);

    std::cout << "Output image: '" << options.rendering_output_filename
              << std::endl;

    return 0;
}