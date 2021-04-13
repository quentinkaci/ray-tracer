#include "core/engine.hh"
#include "scene/scene.hh"
#include "utils/parser.hh"

#include "scene/objects/plane.hh"
#include "scene/objects/sphere.hh"
#include "scene/textures/transparent_texture.hh"
#include "scene/textures/uniform_texture.hh"

#include "scene/point_light.hh"

#include "utils/image.hh"

using namespace scene;
using namespace core;
using namespace utils;
using namespace primitives;

static double random_double() { return (double)rand() / RAND_MAX; }

static Color random_color()
{
    return Color(static_cast<uchar>(fabs(255 * random_double())),
                 static_cast<uchar>(fabs(255 * random_double())),
                 static_cast<uchar>(fabs(255 * random_double())));
}

int main(int, char*[])
{
    Scene   scene;
    Options options;

    //    if (argc > 1)
    //        parse_json(argv[1], options, scene);

    auto image = create_image(options.rendering_output_filename);
    image->create(options.rendering_width, options.rendering_height);

    scene.camera = Camera(
        Point3(13, 1.5, 3), Point3(0, 0, 0), Point3(0, 1, 0), 62.22, 35, 1);

    scene.light_sources.emplace_back(
        std::make_shared<PointLight>(Point3(13, 10, 3), Color(255, 255, 255)));

    //    scene.light_sources.emplace_back(
    //        std::make_shared<PointLight>(Point3(25, 1000, 25), Color(255, 255,
    //        255)));
    //
    //    scene.light_sources.emplace_back(
    //        std::make_shared<PointLight>(Point3(-25, 1000, -25), Color(255,
    //        255, 255)));

    scene.objects.emplace_back(std::make_shared<Plane>(
        std::make_shared<UniformTexture>(
            UniformTexture(TextureMaterialCaracteristics{
                0.005, 0.30, 1, 0.05, Color(255, 127, 80)})),
        Point3(0, 0, 0),
        Vector3(0, 1, 0)));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            double radius     = 0.1 + 0.15 * random_double();
            auto   choose_mat = random_double();
            Point3 center(a + 2 * random_double() + 2,
                          radius,
                          b + 2 * random_double() + 2);

            if (Vector3(center - Point3(8, radius, 1)).norm() > 1 &&
                Vector3(center - Point3(-3, radius, 6)).norm() > 1)
            {
                if (choose_mat < 0.8)
                {
                    scene.objects.emplace_back(std::make_shared<Sphere>(
                        std::make_shared<UniformTexture>(
                            UniformTexture(TextureMaterialCaracteristics{
                                0.005, 0.30, 1, 0.001, random_color()})),
                        center,
                        radius));
                }
                else if (choose_mat < 0.95)
                {
                    scene.objects.emplace_back(std::make_shared<Sphere>(
                        std::make_shared<UniformTexture>(
                            UniformTexture(TextureMaterialCaracteristics{
                                0.005, 0.30, 1, 0.4, random_color()})),
                        center,
                        radius));
                }
                else
                {
                    scene.objects.emplace_back(std::make_shared<Sphere>(
                        std::make_shared<TransparentTexture>(
                            TransparentTexture(1.5,
                                               TextureMaterialCaracteristics{
                                                   0., 0., 0., 1, Color()})),
                        center,
                        radius));
                }
            }
        }
    }

    scene.objects.emplace_back(std::make_shared<Sphere>(
        std::make_shared<TransparentTexture>(TransparentTexture(
            1.5, TextureMaterialCaracteristics{0., 0., 0., 1, Color()})),
        Point3(8, 0.6, 1),
        0.6));

    scene.objects.emplace_back(std::make_shared<Sphere>(
        std::make_shared<UniformTexture>(
            UniformTexture(TextureMaterialCaracteristics{
                0.005, 0.30, 1, 0.5, random_color()})),
        Point3(-3, 0.6, 6),
        0.6));

    Engine engine(options, scene);
    engine.run(*image);

    image->save(options.rendering_output_filename);

    std::cout << "Output image: '" << options.rendering_output_filename
              << std::endl;

    return 0;
}