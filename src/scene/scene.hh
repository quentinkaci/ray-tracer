#pragma once

#include "camera.hh"
#include "light.hh"
#include "object.hh"

#include <vector>

namespace scene
{
struct Scene
{
    Scene() = default;

    ~Scene() = default;

    std::vector<Object*> objects;
    std::vector<Light*>  light_sources;
    Camera               camera;
};
} // namespace scene