#include "scene/objects/cube_aligned.hh"

#include <cmath>

namespace scene
{
CubeAligned::CubeAligned(
    const std::shared_ptr<TextureMaterial>& texture_material,
    const primitives::Point3&               pos,
    double                                  size)
    : Object(texture_material)
    , min_(pos.x - size / 2, pos.y - size / 2, pos.z - size / 2)
    , max_(pos.x + size / 2, pos.y + size / 2, pos.z + size / 2)
{
}

static void swap(double& a, double& b)
{
    double tmp = a;
    a          = b;
    b          = tmp;
}

std::optional<double>
CubeAligned::ray_intersection(const primitives::Point3&  A,
                              const primitives::Vector3& v) const
{
    double tmin = (min_.x - A.x) / v.x;
    double tmax = (max_.x - A.x) / v.x;

    if (tmin > tmax)
        swap(tmin, tmax);

    double tymin = (min_.y - A.y) / v.y;
    double tymax = (max_.y - A.y) / v.y;

    if (tymin > tymax)
        swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return std::nullopt;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    double tzmin = (min_.z - A.z) / v.z;
    double tzmax = (max_.z - A.z) / v.z;

    if (tzmin > tzmax)
        swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return std::nullopt;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return tmin;
}

primitives::Vector3 CubeAligned::get_normal(const primitives::Point3&,
                                            const primitives::Vector3&) const
{
    return primitives::Vector3();
}

primitives::Point3
CubeAligned::get_planar_projection(const primitives::Point3&) const
{
    return primitives::Point3();
}

TextureMaterialCaracteristics
CubeAligned::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene