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
Rectangle::get_planar_projection(const primitives::Point3& A) const
{
    primitives::Vector3 normal = get_normal(A, primitives::Vector3(0, 0, 0));

    primitives::Vector3 e1 =
        normal.cross(primitives::Vector3(1, 0, 0)).normalize();

    if (e1 == primitives::Vector3(0, 0, 0))
    {
        e1 = normal.cross(primitives::Vector3(0, 0, 1)).normalize();
    }

    primitives::Vector3 e2 = normal.cross(e1).normalize();

    double u = e1.dot(A);
    double v = e2.dot(A);

    return primitives::Point3(u, v, 0);
}

TextureMaterialCaracteristics
Rectangle::get_texture(const primitives::Point3& A) const
{
    return Object::texture_material_.get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene