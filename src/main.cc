#include "engine.hh"
#include "scene/scene.hh"
#include "utils/parser.hh"

using namespace scene;
using namespace core;
using namespace utils;

int main(int argc, char* argv[])
{
    Scene   scene;
    Options options;

    if (argc > 1)
        parse_json(argv[1], options, scene);

    Engine engine(options, scene);
    Image image = engine.run(options.rendering_height, options.rendering_width);

    image.save_to_ppm(options.rendering_output_filename);

    std::cout << "Output image: '" << options.rendering_output_filename
              << ".ppm'" << std::endl;

    return 0;
}