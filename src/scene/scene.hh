#pragma once

#include "camera.hh"
#include "light.hh"
#include "scene/objects/englobing_object.hh"
#include "scene/objects/object.hh"

#include <memory>
#include <vector>

namespace scene
{
struct Scene
{
    Scene() = default;

    ~Scene() = default;

    std::vector<std::shared_ptr<Object>>          objects;
    std::vector<std::shared_ptr<EnglobingObject>> englobing_objects;
    std::vector<std::shared_ptr<Light>>           light_sources;
    Camera                                        camera;
};
} // namespace scene