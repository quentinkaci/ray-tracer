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
    primitives::Vector3
    compute_depth_of_field(const primitives::Point3&  origin,
                           const primitives::Vector3& vector);

    primitives::Vector3
    compute_anti_aliasing(const primitives::Point3&             origin,
                          const primitives::Vector3&            vector,
                          const std::vector<primitives::Color>& neighbours);

    std::optional<primitives::Vector3> cast_ray(const primitives::Point3&  A,
                                                const primitives::Vector3& v,
                                                uint depth = 0);

    scene::Scene scene_;

    const scene::Object* last_reflected_object_ = nullptr;

    std::default_random_engine re_;

    std::uniform_real_distribution<double> dof_unif_x_;
    std::uniform_real_distribution<double> dof_unif_y_;

    std::uniform_real_distribution<double> aa_unif_x_;
    std::uniform_real_distribution<double> aa_unif_y_;
};
} // namespace engine
