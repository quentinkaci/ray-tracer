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

    double normal_dot_ray = normal.dot(v);
    if (fabs(normal_dot_ray) < EPSILON)
        return std::nullopt;

    double d = normal.dot(primitives::Vector3(p0_));

    double lambda = (normal.dot(primitives::Vector3(A)) + d) / normal_dot_ray;
    if (lambda < 0)
        return std::nullopt;

    primitives::Vector3 intersection = primitives::Vector3(A) + v * lambda;

    // Check if outside of first edge
    primitives::Vector3 edge0(p1_ - p0_);
    primitives::Vector3 p0_to_intersection =
        intersection - primitives::Vector3(p0_);
    if (normal.dot(edge0.cross(p0_to_intersection)) < 0)
        return std::nullopt;

    // Check if outside of second edge
    primitives::Vector3 edge1(p2_ - p1_);
    primitives::Vector3 p1_to_intersection =
        intersection - primitives::Vector3(p1_);
    if (normal.dot(edge1.cross(p1_to_intersection)) < 0)
        return std::nullopt;

    // Check if outside of third edge
    primitives::Vector3 edge2(p0_ - p2_);
    primitives::Vector3 p2_to_intersection =
        intersection - primitives::Vector3(p2_);
    if (normal.dot(edge2.cross(p2_to_intersection)) < 0)
        return std::nullopt;

    return lambda;
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
Triangle::get_planar_projection(const primitives::Point3&) const
{
    // FIXME Uniform texture
    return primitives::Point3();
}

TextureMaterialCaracteristics
Triangle::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(A);
}
} // namespace scene