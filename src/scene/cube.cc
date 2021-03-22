#include "cube.hh"
#include "rectangle.hh"

#include <cmath>

#define EPSILON 0.0001

namespace scene
{
Cube::Cube(const TextureMaterial&    texture_material,
           const primitives::Point3& pos,
           double                    size)
    : Object(texture_material)
{
    auto a1 = pos + primitives::Point3(size / 2, size / 2, -size / 2);
    auto a2 = pos + primitives::Point3(size / 2, size / 2, size / 2);
    auto a3 = pos + primitives::Point3(-size / 2, size / 2, size / 2);
    auto a4 = pos + primitives::Point3(-size / 2, size / 2, -size / 2);
    auto a5 = pos + primitives::Point3(-size / 2, -size / 2, -size / 2);
    auto a6 = pos + primitives::Point3(size / 2, -size / 2, -size / 2);
    auto a7 = pos + primitives::Point3(size / 2, -size / 2, size / 2);
    auto a8 = pos + primitives::Point3(-size / 2, -size / 2, size / 2);

    rectangles_.emplace_back(texture_material_, a4, a3, a2, a1); // top
    rectangles_.emplace_back(texture_material_, a5, a8, a7, a6); // bottom
    rectangles_.emplace_back(texture_material_, a5, a4, a1, a6); // front
    rectangles_.emplace_back(texture_material_, a8, a3, a2, a7); // back
    rectangles_.emplace_back(texture_material_, a5, a4, a3, a8); // left
    rectangles_.emplace_back(texture_material_, a6, a1, a2, a7); // right
}

std::optional<double> Cube::ray_intersection(const primitives::Point3&  A,
                                             const primitives::Vector3& v) const
{
    std::optional<double> res = std::nullopt;

    for (Rectangle r : rectangles_)
    {
        std::optional<double> t = r.ray_intersection(A, v);
        if (t.has_value() && (!res.has_value() || t < res.value()))
            res = t;
    }

    return res;
}

primitives::Vector3 Cube::get_normal(const primitives::Point3&  A,
                                     const primitives::Vector3& v) const
{
    for (Rectangle r : rectangles_)
    {
        primitives::Vector3 normal = r.get_normal(A, v);
        primitives::Vector3 A_pos  = A - r.pos_;
        if (A_pos.dot(normal) == 0)
            return normal;
    }

    // Should not happen as hitpoint is on one of the 6 sides.
    return primitives::Vector3();
}

primitives::Point3
Cube::get_planar_projection(const primitives::Point3& A) const
{
    for (Rectangle r : rectangles_)
    {
        primitives::Vector3 normal = r.get_normal(A, primitives::Vector3());
        primitives::Vector3 A_pos  = A - r.pos_;
        if (A_pos.dot(normal) == 0)
            return r.get_planar_projection(A);
    }

    // Should not happen as hitpoint is on one of the 6 sides.
    return primitives::Point3();
}

TextureMaterialCaracteristics
Cube::get_texture(const primitives::Point3& A) const
{
    return Object::texture_material_.get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene