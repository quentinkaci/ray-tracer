#include "plane.hh"

#include <cmath>

#define PLANE_EPSILON 1e-6

namespace scene
{
Plane::Plane(const std::shared_ptr<TextureMaterial>& texture_material,
             const primitives::Point3&               pos,
             const primitives::Vector3&              normal)
    : Object(texture_material)
    , pos_(pos)
    , normal_(normal)
{
}

std::optional<double>
Plane::ray_intersection(const primitives::Point3&  A,
                        const primitives::Vector3& v) const
{
    double denom = normal_.dot(v);

    if (std::abs(denom) > PLANE_EPSILON)
    {
        primitives::Vector3 pos_A = pos_ - A;

        double t = pos_A.dot(normal_) / denom;

        if (t >= 0)
            return t;
    }
    return std::nullopt;
}

primitives::Vector3 Plane::get_normal(const primitives::Point3&,
                                      const primitives::Vector3&) const
{
    // FIXME: adapt normal depending on direction of ray.
    return normal_;
}

primitives::Point3
Plane::get_planar_projection(const primitives::Point3& A) const
{
    double trash = 0;
    double u     = std::modf(A.x, &trash);
    double v     = std::modf(A.z, &trash);

    return primitives::Point3(u, v, 0);
}

TextureMaterialCaracteristics
Plane::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene