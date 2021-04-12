#pragma once

#include "object.hh"
#include "scene/scene.hh"

namespace scene
{
class Billboard : public Object
{
  public:
    Billboard(const std::shared_ptr<TextureMaterial>& texture_material,
              const Scene&                            scene,
              const primitives::Point3&               pos,
              const double                            width,
              const double                            height);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3 get_planar_projection(const primitives::Point3& A) const;

    TextureMaterialCaracteristics
    get_texture_info(const primitives::Point3& A) const override;

  private:
    const primitives::Point3  pos_;
    const primitives::Vector3 normal_;
    const double              width_;
    const double              height_;
};
} // namespace scene