#pragma once

#include "scene/scene.hh"
#include "sphere.hh"

#include <memory>

namespace scene
{
static constexpr double SKYBOX_RADIUS = 100.0;

class Skybox : public Object
{
  public:
    Skybox(const std::shared_ptr<TextureMaterial>& texture_material,
           const Scene&                            scene);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3
    get_planar_projection(const primitives::Point3& A) const override;

    TextureMaterialCaracteristics
    get_texture_info(const primitives::Point3& A) const override;

  private:
    const Sphere sphere_;
};
} // namespace scene