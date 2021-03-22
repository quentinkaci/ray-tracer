#include "plane.hh"

#include <cmath>

#define PLANE_EPSILON 1e-6

namespace scene
{
Plane::Plane(const TextureMaterial&     texture_material,
             const primitives::Point3&  pos,
             const primitives::Vector3& normal)
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
    primitives::Vector3 e1 =
        normal_.cross(primitives::Vector3(1, 0, 0)).normalize();

    if (e1 == primitives::Vector3(0, 0, 0))
    {
        e1 = normal_.cross(primitives::Vector3(0, 0, 1)).normalize();
    }

    primitives::Vector3 e2 = normal_.cross(e1).normalize();

    double u = e1.dot(A);
    double v = e2.dot(A);

    return primitives::Point3(u, v, 0);
}

TextureMaterialCaracteristics
Plane::get_texture(const primitives::Point3& A) const
{
    return Object::texture_material_.get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene