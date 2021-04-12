#pragma once

#include <vector>

#include "object.hh"
#include "rectangle.hh"

namespace scene
{
enum CUBE_FACES
{
    FRONT = 0,
    BACK,
    TOP,
    DOWN,
    LEFT,
    RIGHT
};

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

    CUBE_FACES get_cube_face(const primitives::Point3& A) const;

  private:
    const primitives::Point3 center_pos_;
    const double             size_;
    const primitives::Point3 min_pos_;
    const primitives::Point3 max_pos_;
};
} // namespace scene