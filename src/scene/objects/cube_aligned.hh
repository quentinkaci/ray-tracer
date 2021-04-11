#pragma once

#include <vector>

#include "object.hh"
#include "rectangle.hh"

namespace scene
{
class CubeAligned : public Object
{
  public:
    CubeAligned(const std::shared_ptr<TextureMaterial>& texture_material,
                const primitives::Point3&               pos,
                double                                  size);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3 get_planar_projection(const primitives::Point3& A) const;

    TextureMaterialCaracteristics
    get_texture_info(const primitives::Point3& A) const override;

  private:
    const primitives::Point3 min_;
    const primitives::Point3 max_;
};
} // namespace scene