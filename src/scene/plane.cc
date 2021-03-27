#include "plane.hh"

#include <cmath>

namespace scene
{
Plane::Plane(const TextureMaterial &texture_material, const primitives::Point3 &pos, const primitives::Vector3 &normal)
    : Object(texture_material), pos_(pos), normal_(normal)
{
}

std::optional<double> Plane::ray_intersection(const primitives::Point3 &A, const primitives::Vector3 &v) const
{
    double denom = normal_.dot(v);

    // std::cout << "normal" << normal_ << std::endl;
    // std::cout << "ray" << v << std::endl;
    // std::cout << std::endl;

    if (std::abs(denom) > 1e-6)
    {
        primitives::Vector3 p0l0 = pos_ - A;
        double t = p0l0.dot(normal_) / denom;

        if (t >= 0)
            return t;
    }
    return std::nullopt;
}

primitives::Vector3 Plane::get_normal(const primitives::Point3 &) const
{
    // FIXME: adapt normal depending on direction of ray.
    return normal_;
}

primitives::Point3 Plane::get_planar_projection(const primitives::Point3 &A) const
{
    return A;
}

TextureMaterialCaracteristics Plane::get_texture(const primitives::Point3 &A) const
{
    return Object::texture_material_.get_caracteristics(get_planar_projection(A));
}
} // namespace scene