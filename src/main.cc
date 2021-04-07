#include "engine.hh"
#include "scene/blob.hh"
#include "scene/bump_mapping_texture.hh"
#include "scene/camera.hh"
#include "scene/cube.hh"
#include "scene/image_texture.hh"
#include "scene/perlin_texture.hh"
#include "scene/plane.hh"
#include "scene/point_light.hh"
#include "scene/procedural_texture.hh"
#include "scene/rectangle.hh"
#include "scene/scene.hh"
#include "scene/sphere.hh"
#include "scene/triangle.hh"
#include "scene/uniform_texture.hh"
#include "utils/parser.hh"

using namespace primitives;
using namespace scene;
using namespace core;
using namespace utils;

int main(int argc, char* argv[])
{
    // FOV 70
    double beta  = 35.;
    double alpha = beta * 16. / 9.;

    Scene scene(Camera(Point3(0., 0., 0.),
                       Point3(0., 0, 1.),
                       Point3(0., 1., 0.),
                       alpha,
                       beta,
                       1.));

    UniformTexture sphere_uniform_texture_red(
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    scene.objects.emplace_back(
        new Sphere(sphere_uniform_texture_red, Point3(1., 1, 10), 2.));

    UniformTexture sphere_uniform_texture_green(
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(0, 150, 0)});
    scene.objects.emplace_back(
        new Sphere(sphere_uniform_texture_green, Point3(-4., -1, 20), 5.));

    scene.light_sources.emplace_back(
        new PointLight(Point3(10., 10., -10.), Color(255, 255, 255)));

    Options options;
    if (argc > 1)
        parse_json(argv[1], options, scene);

    Engine engine(options, scene);
    Image  image = engine.run(1080, 1920);

    image.save_to_ppm("rendered_scene");

    return 0;
}