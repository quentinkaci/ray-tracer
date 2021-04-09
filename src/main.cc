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
#include "scene/transparent_texture.hh"
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

    ProceduralTexture proc_texture(TextureMaterialCaracteristics{
        0.005, 0.30, 1., 0.30, Color(10, 10, 200)});

    // Assume we are running the executable in a build folder
    ImageTexture sphere_billiard_texture(
        *Image::load_from_ppm("../textures/8_billiard_ball"),
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color()});
    scene.objects.emplace_back(
        new Sphere(sphere_billiard_texture, Point3(0., -2.5, 15.), 2.));

    UniformTexture uniform_texture_red(
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0., Color(150, 0, 0)});

    UniformTexture uniform_texture_green(TextureMaterialCaracteristics{
        0.005, 0.30, 1., 0., Color(153, 168, 30)});

    UniformTexture uniform_texture_blue(
        TextureMaterialCaracteristics{0.005, 0.30, 1., 0., Color(10, 10, 200)});

    TransparentTexture transparent_texture(
        TextureMaterialCaracteristics{0., 0., 0., 1., Color(10, 10, 200)});

    // scene.objects.emplace_back(
    //     new Sphere(uniform_texture_blue, Point3(-2.5, 0., 10.), 1.0));

    // scene.objects.emplace_back(
    //     new Sphere(uniform_texture_red, Point3(2.5, 0., 10.), 1.0));

    // scene.objects.emplace_back(
    //     new Sphere(transparent_texture, Point3(0., 0., 10.), 0.8));

    // scene.objects.emplace_back(new Plane(
    // uniform_texture_green, Point3(0., -1.5, 0.), Vector3(0, 1, 0)));

    // scene.objects.emplace_back(
    //     new Sphere(uniform_texture_green, Point3(0., -11, 20.), 10.));

    std::cout << "Rendering " << scene.objects.size() << " objects"
              << std::endl;

    scene.light_sources.emplace_back(
        new PointLight(Point3(10., 10., -10.), Color(255, 255, 255)));

    Engine engine(scene);
    Image  image = engine.run(1080, 1920);

    image.save_to_ppm("rendered_scene");

    return 0;
}