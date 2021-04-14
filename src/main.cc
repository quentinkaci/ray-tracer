#include "core/engine.hh"
#include "scene/scene.hh"
#include "utils/parser.hh"

#include "primitives/point.hh"
#include "scene/blob.hh"
#include "scene/objects/plane.hh"
#include "scene/objects/skybox.hh"
#include "scene/point_light.hh"
#include "scene/textures/image_texture.hh"
#include "scene/textures/uniform_texture.hh"
#include "utils/perlin_noise.hh"

#include "utils/image.hh"

#include <algorithm>

using namespace scene;
using namespace core;
using namespace utils;
using namespace primitives;

int main(int, char*[])
{
    Scene   scene;
    Options options;

    scene.camera = Camera(
        Point3(0, 0, 0), Point3(0.2, 0, 1), Point3(0, 1, 0), 62.22, 35, 1);

    scene.light_sources.emplace_back(
        std::make_shared<PointLight>(Point3(10, 4, -10), Color(255, 255, 255)));

    PerlinNoise perlin;
    Blob        blob(Point3(-2, -2, 2),
              0.1,
              10,
              400.,
              std::make_shared<UniformTexture>(TextureMaterialCaracteristics{
                  0.005, 0.30, 1, 0.05, Color(0, 150, 0)}));

    std::vector<double> perlin_2d;
    for (double j = -2; j < 2; j += 0.1)
    {
        for (double i = -2; i < 2; i += 0.1)
        {
            perlin_2d.emplace_back(perlin.get_octave(i, j, 0, 5, 10));
        }
    }

    double max = *std::max_element(perlin_2d.begin(), perlin_2d.end());

    int k = 0;
    for (double i = -2; i < 2; i += 0.1)
    {
        for (double z = 2; z < 4; z += 0.1)
        {
            if (perlin_2d[k++] > 0.002 * max)
            {
                Point3 energy(i, -0.6, z);
                blob.add(energy);
            }
        }
    }

    blob.run(scene);

    const auto water = create_image("../textures/water_8.ppm");
    water->load("../textures/water_8.ppm");

    scene.objects.emplace_back(std::make_shared<Plane>(
        std::make_shared<ImageTexture>(
            water,
            0,
            TextureMaterialCaracteristics{
                0.005, 0.30, 1, 0.05, Color(0, 0, 150)}),
        Point3(0, -0.6, 2),
        Vector3(0, 1, 0)));

    //    const auto night_sky = create_image("../textures/stars.png");
    //    night_sky->load("../textures/stars.png");
    //
    //    scene.objects.emplace_back(std::make_shared<Skybox>(
    //        std::make_shared<ImageTexture>(
    //            night_sky,
    //            100,
    //            TextureMaterialCaracteristics{
    //                0.005, 0.30, 1, 0.05, Color(0, 0, 150)}),
    //        scene));

    //    if (argc > 1)
    //        parse_json(argv[1], options, scene);

    auto image = create_image(options.rendering_output_filename);
    image->create(options.rendering_width, options.rendering_height);

    Engine engine(options, scene);
    engine.run(*image);

    image->save(options.rendering_output_filename);

    std::cout << "Output image: '" << options.rendering_output_filename
              << std::endl;

    return 0;
}