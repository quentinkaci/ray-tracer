#include "scene/scene.hh"
#include "scene/uniform_texture.hh"
#include "scene/sphere.hh"
#include "scene/camera.hh"

#include "engine.hh"


using namespace primitives;
using namespace scene;
using namespace engine;
using namespace utils;


int main(void)
{
    Scene scene(Camera(Point3(0, 0, 0), Point3(0, 0, 50), Point3(0, 1, 0), 90, 90, 1));

    UniformTexture sphere_uniform_texture(TextureMaterialCaracteristics{1, 1, Color(255, 0, 0)});
    scene.objects.emplace_back(new Sphere(sphere_uniform_texture, Point3(0, 0, 50), 0.5));

    Engine engine(scene);
    Image image = engine.run(1080, 1920);

    image.save_to_ppm("first");

    return 0;
}