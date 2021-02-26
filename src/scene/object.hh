#pragma once

#include "texture_material.hh"
#include "primitives/vector.hh"

#include <vector>


namespace scene
{
    class Object
    {
    public:
        Object(const TextureMaterial& texture_material);

        virtual std::vector<int> ray_intersection(const primitives::Point3& A, const primitives::Vector3& v) = 0;

        virtual primitives::Vector3 get_normal(const primitives::Point3& A) = 0;

        virtual primitives::Vector3 get_texture(const primitives::Point3& A) = 0;

    private:
        const TextureMaterial& texture_material_;
    };
}