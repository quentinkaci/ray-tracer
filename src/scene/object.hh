#pragma once

#include "texture_material.hh"
#include "primitives/vector.hh"


namespace scene
{
    class Object
    {
    public:
        Object(const TextureMaterial& texture_material);

        virtual bool is_ray_intersecting(const primitives::Point3& a, int lambda, const primitives::Vector3& v) = 0;

        virtual primitives::Vector3 get_normal_from(const primitives::Point3& p) = 0;

        virtual primitives::Vector3 get_texture(const primitives::Point3& p) = 0;

    private:
        const TextureMaterial& texture_material_;
    };
}