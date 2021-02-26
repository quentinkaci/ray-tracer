#include "sphere.hh"

#include <math.h>


namespace scene
{
    Sphere::Sphere(const TextureMaterial& texture_material, const primitives::Point3& center, const uint radius)
        : Object(texture_material)
        , center_(center)
        , radius_(radius)
    {}

    std::vector<int> Sphere::ray_intersection(const primitives::Point3& A, const primitives::Vector3& v)
    {
        int a = v.dst.x * v.dst.x + v.dst.y * v.dst.y + v.dst.z * v.dst.z;
        int b = 2 * (v.dst.x * (A.x - center_.x) + v.dst.y * (A.y - center_.y)  + v.dst.z * (A.z - center_.z));
        int c = A.x * (A.x - 2 * center_.x) + center_.x * center_.x
                + A.y * (A.y - 2 * center_.y) + center_.y * center_.y
                + A.z * (A.z - 2 * center_.z) + center_.z * center_.z
                - radius_ * radius_;

        std::vector<int> res;

        int delta = b * b - 4 * a * c;
        if (delta < 0)
            return res;
        else if (delta == 0)
            res.push_back(- b / (2 * a));
        else // delta > 0
        {
            res.push_back((-b - sqrt(delta)) / (2 * a));
            res.push_back((-b + sqrt(delta)) / (2 * a));
        }

        return res;
    }

    primitives::Vector3 Sphere::get_normal(const primitives::Point3&)
    {
        // FIXME
        return primitives::Vector3(primitives::Point3(0, 0, 0));
    }

    primitives::Vector3 Sphere::get_texture(const primitives::Point3& A)
    {
        return Object::get_texture(A);
    }
}