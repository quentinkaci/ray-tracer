#pragma once

#include "object.hh"
#include "triangle.hh"

namespace scene
{
class Rectangle : public Object
{
  public:
    Rectangle(const std::shared_ptr<TextureMaterial>& texture_material,
              const primitives::Point3&               p0,
              const primitives::Point3&               p1,
              const primitives::Point3&               p2,
              const primitives::Point3&               p3);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3 get_planar_projection(const primitives::Point3& A) const;

    TextureMaterialCaracteristics
    get_texture_info(const primitives::Point3& A) const override;

    primitives::Point3 pos_;

  private:
    Triangle triangle1_;

    Triangle triangle2_;
};
} // namespace scene