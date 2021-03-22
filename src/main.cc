#include "engine.hh"
#include "scene/blob.hh"
#include "scene/bump_mapping_texture.hh"
#include "scene/camera.hh"
#include "scene/cube.hh"
#include "scene/image_texture.hh"
#include "scene/perlin_texture.hh"
#include "scene/plane.hh"
#include "scene/point_light.hh"
#include "scene/rectangle.hh"
#include "scene/scene.hh"
#include "scene/sphere.hh"
#include "scene/triangle.hh"
#include "scene/uniform_texture.hh"

using namespace primitives;
using namespace scene;
using namespace engine;
using namespace utils;

int main(void)
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

    BumpMappingTexture sphere_bump_texture_red(
        6.,
        10.,
        2.,
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    scene.objects.emplace_back(
        new Sphere(sphere_bump_texture_red, Point3(6., -2.5, 22.5), 2.));

    PerlinTexture perlin_wood_texture(
        3,
        8,
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0.05, Color(150, 0, 0)},
        PerlinTextureType::WOOD,
        std::vector<Color>{Color(90, 70, 0), Color(0, 0, 0)});
    scene.objects.emplace_back(
        new Sphere(perlin_wood_texture, Point3(0., 2.5, 15.), 2.));

    PerlinTexture perlin_texture(
        5,
        10,
        TextureMaterialCaracteristics{0.005, 0.30, 1., 1., Color(150, 0, 0)},
        PerlinTextureType::DEFAULT,
        std::vector<Color>{
            Color(51, 251, 51), Color(255, 153, 51), Color(255, 0, 0)});
    scene.objects.emplace_back(
        new Sphere(perlin_texture, Point3(7.5, 2.5, 22.5), 2.));

    PerlinTexture perlin_marble_texture(
        4,
        10,
        TextureMaterialCaracteristics{0.005, 0.30, 1., 1., Color(150, 0, 0)},
        PerlinTextureType::MARBLE,
        std::vector<Color>{Color(255, 255, 255), Color(0, 0, 0)});
    scene.objects.emplace_back(
        new Sphere(perlin_marble_texture, Point3(-7.5, 2.5, 22.5), 2.));

    // Assume we are running the executable in a build folder
    ImageTexture sphere_earth_texture(
        *Image::load_from_ppm("../textures/earth_map"),
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0., Color()});
    scene.objects.emplace_back(
        new Sphere(sphere_earth_texture, Point3(-6., -2.5, 22.5), 2.));

    // Assume we are running the executable in a build folder
    ImageTexture sphere_billiard_texture(
        *Image::load_from_ppm("../textures/8_billiard_ball"),
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color()});
    // scene.objects.emplace_back(
    //     new Sphere(sphere_billiard_texture, Point3(0., -2.5, 15.), 2.));

    // UniformTexture uniform_texture_red(TextureMaterialCaracteristics{0.005,
    // 0.30, 1., 0.30, Color(150, 0, 0)}); Blob blob(primitives::Point3(-1.,
    // -1., 4.), 0.05, 2, 400, uniform_texture_red);
    // blob.add(primitives::Point3(0., 0., 5.));

    UniformTexture uniform_texture_blue(TextureMaterialCaracteristics{
        0.005, 0.30, 1., 0.30, Color(10, 10, 200)});
    // Blob blob(primitives::Point3(0., -5., 0.), 0.1, 10, 9.9,
    // uniform_texture_blue); blob.add(primitives::Point3(0., -2.5, 8.), 10);
    // blob.run(scene);

    // scene.objects.emplace_back(new Sphere(uniform_texture_red, Point3(-1.,
    // -1., 5.), 0.1)); scene.objects.emplace_back(new
    // Sphere(uniform_texture_red, Point3(-1., 1., 5.), 0.1));
    // scene.objects.emplace_back(new Sphere(uniform_texture_red, Point3(1.,
    // -1., 5.), 0.1)); scene.objects.emplace_back(new
    // Sphere(uniform_texture_red, Point3(1., 1., 5.), 0.1));

    scene.objects.emplace_back(
        new Plane(uniform_texture_blue, Point3(0., -5., 0.), Vector3(0, 1, 0)));

    std::cout << "Rendering " << scene.objects.size() << " objects"
              << std::endl;
    // scene.objects.emplace_back(new Plane(
    //     uniform_texture_blue, Point3(0., -5.0, 0.), Vector3(0, 1, 0)));

    // scene.objects.emplace_back(new Triangle(uniform_texture_blue,
    //                                         Point3(-3.0, 2.4, 15.),
    //                                         Point3(3.0, 2.4, 15.),
    //                                         Point3(-3.0, 2.0, 5.)));

    // scene.objects.emplace_back(new Rectangle(uniform_texture_blue,
    //                                          Point3(1.0, 0., 15.),
    //                                          Point3(1.0, 2.5, 15.),
    //                                          Point3(-1.0, 2.5, 15.),
    //                                          Point3(-1.0, 0., 15.)));

    scene.objects.emplace_back(
        new Cube(uniform_texture_blue, Point3(0.0, -2., 12.), 2.0));

    scene.light_sources.emplace_back(
        new PointLight(Point3(10., 10., -10.), Color(255, 255, 255)));

    Engine engine(scene);
    Image  image = engine.run(1080, 1920);

    image.save_to_ppm("rendered_scene");

    return 0;
}