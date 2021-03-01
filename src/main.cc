#include "scene/scene.hh"
#include "scene/uniform_texture.hh"
#include "scene/sphere.hh"
#include "scene/camera.hh"
#include "scene/point_light.hh"

#include "engine.hh"


using namespace primitives;
using namespace scene;
using namespace engine;
using namespace utils;


int main(void)
{
    Scene scene(Camera(Point3(0., 0., 0.), Point3(0., 0., 1.), Point3(0., 1., 0.), 90., 60., 1.));

    UniformTexture sphere_uniform_texture_red(TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(150, 0, 0)});
    scene.objects.emplace_back(new Sphere(sphere_uniform_texture_red, Point3(2., 0., 5.), 1.7));

    UniformTexture sphere_uniform_texture_green(TextureMaterialCaracteristics{0.005, 0.30, 1., 0.30, Color(0, 0, 150)});
    scene.objects.emplace_back(new Sphere(sphere_uniform_texture_green, Point3(-2., 0., 5.), 1.7));

    scene.light_sources.emplace_back(new PointLight(Point3(10., 10., -10.), Color(255, 255, 255)));

    Engine engine(scene);
    Image image = engine.run(1080, 1920);

    image.save_to_ppm("rendered_scene");

    return 0;
}