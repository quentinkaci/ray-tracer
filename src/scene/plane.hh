#pragma once

#include "object.hh"

namespace scene
{
class Plane : public Object
{
  public:
    Plane(const std::shared_ptr<TextureMaterial>& texture_material,
          const primitives::Point3&               pos,
          const primitives::Vector3&              normal);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3
    get_planar_projection(const primitives::Point3& A) const override;

    TextureMaterialCaracteristics
    get_texture(const primitives::Point3& A) const override;

  private:
    primitives::Point3  pos_;
    primitives::Vector3 normal_;
};
} // namespace scene