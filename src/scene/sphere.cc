#include "sphere.hh"

#include <cmath>


namespace scene
{
    Sphere::Sphere(const TextureMaterial& texture_material, const primitives::Point3& center, const double radius)
        : Object(texture_material)
        , center_(center)
        , radius_(radius)
    {}

    std::optional<double> Sphere::ray_intersection(const primitives::Point3& A, const primitives::Vector3& v) const
    {
        primitives::Vector3 relative_A(A - center_);
        double a = v.dot(v);
        double b = 2 * v.dot(relative_A);
        double c = relative_A.dot(relative_A) - pow(radius_, 2);

        double delta = pow(b, 2) - 4 * a * c;
        if (delta >= 0)
        {
            double x1 = (- b - sqrt(delta)) / (2 * a);
            if (x1 > 0) // It will be the lowest of 2 solutions
                return x1;

            double x2 = (- b + sqrt(delta)) / (2 * a);
            if (x2 > 0)
                return x2;

            return std::nullopt;
        }

        // No intersection
        return std::nullopt;
    }

    primitives::Vector3 Sphere::get_normal(const primitives::Point3& A) const
    {
        return primitives::Vector3((A - center_) / radius_);
    }

    TextureMaterialCaracteristics Sphere::get_texture(const primitives::Point3& A) const
    {
        double u = (atan2(- (A.z - center_.z), A.x - center_.x) + M_PI) / (2 * M_PI);
        double v = acos(- (A.y - center_.y) / radius_) / M_PI;

        return Object::texture_material_.get_caracteristics(primitives::Point3(1 - u, 1- v, 0));
    }
}