#pragma once

#include "object.hh"

namespace scene
{
class Triangle : public Object
{
  public:
    Triangle(const std::shared_ptr<TextureMaterial>& texture_material,
             const primitives::Point3&               p0,
             const primitives::Point3&               p1,
             const primitives::Point3&               p2);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3 get_planar_projection(const primitives::Point3& A) const;

    TextureMaterialCaracteristics
    get_texture(const primitives::Point3& A) const override;

  private:
    primitives::Point3 p0_;

    primitives::Point3 p1_;

    primitives::Point3 p2_;
};
} // namespace scene