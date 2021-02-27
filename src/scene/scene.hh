#pragma once

#include "object.hh"
#include "light.hh"
#include "camera.hh"

#include <vector>

namespace scene
{
    struct Scene
    {
        Scene() = default;

        ~Scene() = default;

        std::vector<Object> objects;
        std::vector<Light> light_sources;
        Camera camera;
    };
}