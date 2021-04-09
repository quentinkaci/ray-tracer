#pragma once

#include "primitives/vector.hh"
#include "texture_material.hh"

#include <memory>
#include <optional>

namespace scene
{
class Object
{
  public:
    explicit Object(std::shared_ptr<TextureMaterial> texture_material);

    virtual std::optional<double>
    ray_intersection(const primitives::Point3&  A,
                     const primitives::Vector3& v) const = 0;

    primitives::Vector3 get_computed_normal(const primitives::Point3&  A,
                                            const primitives::Vector3& v) const;

    virtual primitives::Vector3
    get_normal(const primitives::Point3&  A,
               const primitives::Vector3& v) const = 0;

    // Z component of result is useless because a planar projection is in 2D
    virtual primitives::Point3
    get_planar_projection(const primitives::Point3& A) const = 0;

    virtual const std::shared_ptr<TextureMaterial>& get_texture() const;

    virtual TextureMaterialCaracteristics
    get_texture_info(const primitives::Point3& A) const = 0;

  protected:
    const std::shared_ptr<TextureMaterial> texture_material_;
};
} // namespace scene