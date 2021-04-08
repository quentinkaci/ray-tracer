#pragma once

#include "camera.hh"
#include "light.hh"
#include "object.hh"

#include <memory>
#include <vector>

namespace scene
{
struct Scene
{
    Scene() = default;

    ~Scene() = default;

    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>>  light_sources;
    Camera                               camera;
};
} // namespace scene