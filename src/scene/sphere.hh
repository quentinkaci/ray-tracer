#pragma once

#include "object.hh"

#include <vector>


namespace scene
{
    class Sphere : Object
    {
    public:
        Sphere(const TextureMaterial& texture_material, const primitives::Point3& center, const uint radius);

        std::vector<int> ray_intersection(const primitives::Point3& A, const primitives::Vector3& v) override;

        primitives::Vector3 get_normal(const primitives::Point3& A) override;

        primitives::Vector3 get_texture(const primitives::Point3& A) override;

    private:
        const primitives::Point3& center_;

        const uint radius_;
    };
}