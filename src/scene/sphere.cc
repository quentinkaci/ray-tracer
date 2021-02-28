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

        // std::cout << "sphere center: " << center_ << std::endl;
        // std::cout << "sphere radius: " << radius_ << std::endl;
        // std::cout << "ray origin: " << A << std::endl;
        // std::cout << "ray direction: " << v << std::endl;

        double delta = pow(b, 2) - 4 * a * c;
        if (delta >= 0)
        {
            double x1 = (- b - sqrt(delta)) / (2 * a);
            double x2 = (- b + sqrt(delta)) / (2 * a);

            if (x1 < 0 && x2 < 0)
                return std::nullopt;
            if (x1 < 0 && x2 >= 0)
                return x2;
            else if (x2 < 0 && x1 >= 0)
                return x1;
            else
                return std::min(x1, x2);
        }

        return std::nullopt;
    }

    primitives::Vector3 Sphere::get_normal(const primitives::Point3& A) const
    {
        return primitives::Vector3((A - center_) / radius_);
    }

    const TextureMaterialCaracteristics& Sphere::get_texture(const primitives::Point3& A) const
    {
        return Object::texture_material_.get_caracteristics(A);
    }
}