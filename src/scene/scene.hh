#pragma once

#include "object.hh"
#include "light.hh"
#include "camera.hh"

#include <vector>

namespace scene
{
    struct Scene
    {
        Scene(Camera camera);

        ~Scene() = default;

        std::vector<Object*> objects;
        std::vector<Light*> light_sources;
        Camera camera;
    };
}