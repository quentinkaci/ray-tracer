#include "scene/scene.hh"
#include "scene/uniform_texture.hh"
#include "scene/image_texture.hh"
#include "scene/sphere.hh"
#include "scene/camera.hh"
#include "scene/point_light.hh"

#include <algorithm>
#include "utils/perlin_noise.hh"

#include "engine.hh"

using namespace primitives;
using namespace scene;
using namespace engine;
using namespace utils;

static void test_perlin_noise()
{
    float size = 1024.0f;

    PerlinNoise p;
    Image image(size, size);

    for (uint x = 0; x < size; x++)
    {
        for (uint y = 0; y < size; y++)
        {
            // double v = p.get(x / size, y / size, 0.5f);
            double v = p.get_octave(x / size, y / size, 0.5f, 5, 1.0f);
            image.pixel(x, y) = primitives::Color(
                std::clamp(v * 255.0f, 0., 255.),
                std::clamp(v * 255.0f, 0., 255.),
                std::clamp(v * 255.0f, 0., 255.));
        }
    }

    image.save_to_ppm("test_perlin_noise");
}

int main(void)
{
    // FOV 70
    double beta = 35.;
    double alpha = beta * 16. / 9.;

    Scene scene(Camera(Point3(0., 0., 0.), Point3(0., 0., 1.), Point3(0., 1., 0.), alpha, beta, 1.));

    UniformTexture sphere_uniform_texture_red(TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    scene.objects.emplace_back(new Sphere(sphere_uniform_texture_red, Point3(0., 2.5, 15.), 2.));

    // Assume we are running the executable in a build folder
    ImageTexture sphere_earth_texture(*Image::load_from_ppm("../textures/earth_map"),
                                      TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color()});
    scene.objects.emplace_back(new Sphere(sphere_earth_texture, Point3(-4., -2.5, 22.5), 2.));

    // Assume we are running the executable in a build folder
    ImageTexture sphere_billiard_texture(*Image::load_from_ppm("../textures/8_billiard_ball"),
                                         TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color()});
    scene.objects.emplace_back(new Sphere(sphere_billiard_texture, Point3(2., -2.5, 15.), 2.));

    scene.light_sources.emplace_back(new PointLight(Point3(10., 10., -10.), Color(255, 255, 255)));

    Engine engine(scene);
    Image image = engine.run(1080, 1920);

    image.save_to_ppm("rendered_scene");

    return 0;
}