#pragma once

#include "primitives/color.hh"
#include "scene/scene.hh"
#include "utils/image.hh"

#include <optional>
#include <random>

namespace engine
{
class Engine
{
  public:
    Engine(const scene::Scene& scene);

    utils::Image run(uint height, uint width);

  private:
    std::optional<primitives::Vector3> cast_ray(const primitives::Point3&  A,
                                                const primitives::Vector3& v,
                                                uint depth = 0);

    bool cast_ray_light_check(const primitives::Point3&  A,
                              const primitives::Vector3& v);

    scene::Scene scene_;

    const scene::Object* last_reflected_object_ = nullptr;

    std::default_random_engine re_;
};
} // namespace engine
