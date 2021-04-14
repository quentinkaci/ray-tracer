#pragma once

#include <vector>

#include "cube_aligned.hh"
#include "object.hh"

namespace scene
{
class CuboidAligned : public Object
{

  public:
    CuboidAligned(const std::shared_ptr<TextureMaterial>& texture_material,
                  const primitives::Point3&               pos,
                  const primitives::Point3&               size);

    std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const override;

    primitives::Vector3 get_normal(const primitives::Point3&  A,
                                   const primitives::Vector3& v) const override;

    primitives::Point3 get_planar_projection(const primitives::Point3& A) const;

    TextureMaterialCaracteristics
    get_texture_info(const primitives::Point3& A) const override;

    CUBE_FACES get_cube_face(const primitives::Point3& A) const;

  private:
    const primitives::Point3 center_pos_;
    const primitives::Point3 size_;
    const primitives::Point3 min_pos_;
    const primitives::Point3 max_pos_;
};
} // namespace scene