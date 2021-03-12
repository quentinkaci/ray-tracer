#pragma once

#include "texture_material.hh"
#include "primitives/vector.hh"

#include <optional>


namespace scene
{
    class Object
    {
    public:
        Object(const TextureMaterial& texture_material);

        virtual std::optional<double> ray_intersection(const primitives::Point3& A, const primitives::Vector3& v) const = 0;

        virtual primitives::Vector3 get_normal(const primitives::Point3& A) const = 0;

        virtual TextureMaterialCaracteristics get_texture(const primitives::Point3& A) const = 0;

    protected:
        const TextureMaterial& texture_material_;
    };
}