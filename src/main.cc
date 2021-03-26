#include "scene/scene.hh"
#include "scene/uniform_texture.hh"
#include "scene/image_texture.hh"
#include "scene/bump_mapping_texture.hh"
#include "scene/perlin_texture.hh"
#include "scene/sphere.hh"
#include "scene/triangle.hh"
#include "scene/camera.hh"
#include "scene/point_light.hh"
#include "engine.hh"
#include "scene/blob.hh"

using namespace primitives;
using namespace scene;
using namespace engine;
using namespace utils;


int main(void)
{
    // FOV 70
    double beta = 35.;
    double alpha = beta * 16. / 9.;

    Scene scene(Camera(Point3(0., 0., 0.), Point3(0., 0., 1.), Point3(0., 1., 0.), alpha, beta, 1.));

    // BumpMappingTexture sphere_bump_texture_red(6., 10., 2., TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    // scene.objects.emplace_back(new Sphere(sphere_bump_texture_red, Point3(6., -2.5, 22.5), 2.));


    // PerlinTexture perlin_wood_texture(3, 8, TextureMaterialCaracteristics{0.005, 0.30, 1., 0.2, Color(150, 0, 0)},
    //     PerlinTextureType::WOOD, std::vector<Color>{Color(90, 70, 0), Color(0, 0, 0)});
    // scene.objects.emplace_back(new Sphere(perlin_wood_texture, Point3(0., 2.5, 15.), 2.));

    // PerlinTexture perlin_texture(5, 10, TextureMaterialCaracteristics{0.005, 0.30, 1., 1., Color(150, 0, 0)},
    //     PerlinTextureType::DEFAULT, std::vector<Color>{Color(51, 251, 51), Color(255, 153, 51), Color(255, 0, 0)});
    // scene.objects.emplace_back(new Sphere(perlin_texture, Point3(7.5, 2.5, 22.5), 2.));

    // PerlinTexture perlin_marble_texture(4, 10, TextureMaterialCaracteristics{0.005, 0.30, 1., 1., Color(150, 0, 0)},
    //     PerlinTextureType::MARBLE, std::vector<Color>{Color(255, 255, 255), Color(0, 0, 0)});
    // scene.objects.emplace_back(new Sphere(perlin_marble_texture, Point3(-7.5, 2.5, 22.5), 2.));


    // // Assume we are running the executable in a build folder
    // ImageTexture sphere_earth_texture(*Image::load_from_ppm("../textures/earth_map"),
    //                                   TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color()});
    // scene.objects.emplace_back(new Sphere(sphere_earth_texture, Point3(-6., -2.5, 22.5), 2.));

    // // Assume we are running the executable in a build folder
    // ImageTexture sphere_billiard_texture(*Image::load_from_ppm("../textures/8_billiard_ball"),
    //                                      TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color()});
    // scene.objects.emplace_back(new Sphere(sphere_billiard_texture, Point3(0., -2.5, 15.), 2.));

    // BumpMappingTexture sphere_bump_texture_red(6., 10., 2., TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    // scene.objects.emplace_back(new Sphere(sphere_bump_texture_red, Point3(0., -2.5, 10.), 2.5));

    UniformTexture uniform_texture_red(TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    Blob blob(primitives::Point3(-1., -1., 4.), 0.05, 2, 400, uniform_texture_red);
    blob.add(primitives::Point3(0., 0., 5.));
    blob.run(scene);

    scene.objects.emplace_back(new Sphere(uniform_texture_red, Point3(-1., -1., 5.), 0.1));
    scene.objects.emplace_back(new Sphere(uniform_texture_red, Point3(-1., 1., 5.), 0.1));
    scene.objects.emplace_back(new Sphere(uniform_texture_red, Point3(1., -1., 5.), 0.1));
    scene.objects.emplace_back(new Sphere(uniform_texture_red, Point3(1., 1., 5.), 0.1));

    scene.light_sources.emplace_back(new PointLight(Point3(10., 10., -10.), Color(255, 255, 255)));

    std::cout << "Rendering " << scene.objects.size() << " objects" << std::endl;

    Engine engine(scene);
    Image image = engine.run(1080, 1920);

    image.save_to_ppm("rendered_scene");

    return 0;
}