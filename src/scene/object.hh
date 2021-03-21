#pragma once

#include "primitives/vector.hh"
#include "texture_material.hh"

#include <optional>

namespace scene
{
class Object
{
  public:
    Object(const TextureMaterial& texture_material);

    virtual std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const = 0;

    primitives::Vector3 get_computed_normal(const primitives::Point3& A) const;

    virtual primitives::Vector3
    get_normal(const primitives::Point3& A) const = 0;

    // Z component of result is useless because a planar projection is in 2D
    virtual primitives::Point3
    get_planar_projection(const primitives::Point3& A) const = 0;

    virtual TextureMaterialCaracteristics
    get_texture(const primitives::Point3& A) const = 0;

  protected:
    const TextureMaterial& texture_material_;
};
} // namespace scene