#include "rectangle.hh"
#include "triangle.hh"

#include <cmath>

#define EPSILON 0.0001

namespace scene
{
Rectangle::Rectangle(const TextureMaterial&    texture_material,
                     const primitives::Point3& p0,
                     const primitives::Point3& p1,
                     const primitives::Point3& p2,
                     const primitives::Point3& p3)
    : Object(texture_material)
    , pos_(p0)
    , triangle1_(texture_material, p0, p1, p2)
    , triangle2_(texture_material, p0, p2, p3)
{
}

std::optional<double>
Rectangle::ray_intersection(const primitives::Point3&  A,
                            const primitives::Vector3& v) const
{
    std::optional<double> t1 = triangle1_.ray_intersection(A, v);
    if (t1.has_value())
        return t1;

    std::optional<double> t2 = triangle2_.ray_intersection(A, v);
    return t2;
}

primitives::Vector3 Rectangle::get_normal(const primitives::Point3&  A,
                                          const primitives::Vector3& v) const
{
    return triangle1_.get_normal(A, v);
}

primitives::Point3
Rectangle::get_planar_projection(const primitives::Point3&) const
{
    // FIXME Uniform texture
    return primitives::Point3();
}

TextureMaterialCaracteristics
Rectangle::get_texture(const primitives::Point3&) const
{
    // FIXME Uniform texture
    return Object::texture_material_.get_caracteristics(primitives::Point3());
}
} // namespace scene