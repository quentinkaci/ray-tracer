#pragma once

#include "primitives/color.hh"
#include "scene/scene.hh"
#include "utils/image.hh"

#include "options.hh"

#include <atomic>
#include <optional>
#include <random>

namespace core
{
class Engine
{
  public:
    Engine(Options options, const scene::Scene& scene);

    void run(utils::Image& image);

  private:
    void render_image(utils::Image& image);

    void apply_gamma_correction(utils::Image& image) const;

    void compute_anaglyph_effect(utils::Image& image);

    void init_distributions(uint height, uint width);

    primitives::Vector3
    compute_depth_of_field(const primitives::Point3&  origin,
                           const primitives::Vector3& vector);

    primitives::Vector3
    compute_anti_aliasing(const primitives::Point3&             origin,
                          const primitives::Vector3&            vector,
                          const std::vector<primitives::Color>& neighbours);

    unsigned int compute_soft_shadow(const primitives::Point3& offset_hitpoint,
                                     const std::shared_ptr<scene::Light>& light,
                                     const primitives::Vector3& light_ray);

    std::optional<primitives::Vector3> cast_ray(const primitives::Point3&  A,
                                                const primitives::Vector3& v,
                                                uint depth = 0);

    bool cast_ray_light_check(const primitives::Point3&  A,
                              const primitives::Vector3& v);

    Options options_;

    scene::Scene scene_;

    std::default_random_engine re_;

    std::uniform_real_distribution<double> dof_unif_x_;
    std::uniform_real_distribution<double> dof_unif_y_;

    std::uniform_real_distribution<double> aa_unif_x_;
    std::uniform_real_distribution<double> aa_unif_y_;

    std::uniform_real_distribution<double> soft_shadow_unif_x_;
    std::uniform_real_distribution<double> soft_shadow_unif_y_;
    std::uniform_real_distribution<double> soft_shadow_unif_z_;

    std::atomic<uint> progress_count_{0};
};
} // namespace core