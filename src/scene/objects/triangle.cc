#include "triangle.hh"

#include <cmath>

#define EPSILON 0.0001

namespace scene
{
Triangle::Triangle(const std::shared_ptr<TextureMaterial>& texture_material,
                   const primitives::Point3&               p0,
                   const primitives::Point3&               p1,
                   const primitives::Point3&               p2)
    : Object(texture_material)
    , p0_(p0)
    , p1_(p1)
    , p2_(p2)
{
}

std::optional<double>
Triangle::ray_intersection(const primitives::Point3&  A,
                           const primitives::Vector3& v) const
{
    primitives::Vector3 normal = get_normal(A, v);

    // Check if ray and plane (defined by normal) are parallel => no
    // intersection
    double normal_dot_ray = normal.dot(v);
    if (std::fabs(normal_dot_ray) < EPSILON)
        return std::nullopt;

    // Determine intersection time
    double t = (normal.dot(primitives::Vector3(A)) +
                normal.dot(primitives::Vector3(p0_))) /
               normal_dot_ray;

    primitives::Vector3 w  = primitives::Vector3(A) + v * t - p0_;
    primitives::Vector3 s1 = p1_ - p0_;
    primitives::Vector3 s2 = p2_ - p0_;

    // Barycentric coordinates
    double b  = (s1.dot(s1) * s2.dot(s2) - pow(s1.dot(s2), 2));
    double b1 = (w.dot(s1) * s2.dot(s2) - w.dot(s2) * s1.dot(s2)) / b;
    double b2 = (w.dot(s2) * s1.dot(s1) - w.dot(s1) * s1.dot(s2)) / b;

    if (b1 >= 0 && b2 >= 0 && b1 + b2 <= 1)
        return t;

    return std::nullopt;
}

primitives::Vector3 Triangle::get_normal(const primitives::Point3&,
                                         const primitives::Vector3& v) const
{
    primitives::Vector3 u0(p1_ - p0_);
    primitives::Vector3 u1(p2_ - p0_);
    primitives::Vector3 normal = u0.cross(u1).normalize();
    if (v.dot(normal) > 0)
        normal = normal * -1.0;
    return normal;
}

primitives::Point3
Triangle::get_planar_projection(const primitives::Point3& A) const
{
    double trash = 0;
    double u     = std::modf(A.x, &trash);
    double v     = std::modf(A.z, &trash);

    return primitives::Point3(u, v, 0);
}

TextureMaterialCaracteristics
Triangle::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene