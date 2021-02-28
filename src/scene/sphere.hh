#pragma once

#include "object.hh"


namespace scene
{
    class Sphere : public Object
    {
    public:
        Sphere(const TextureMaterial& texture_material, const primitives::Point3& center, const double radius);

        std::optional<double> ray_intersection(const primitives::Point3& A, const primitives::Vector3& v) const override;

        primitives::Vector3 get_normal(const primitives::Point3& A) const override;

        const TextureMaterialCaracteristics& get_texture(const primitives::Point3& A) const override;

    private:
        primitives::Point3 center_;

        const double radius_;
    };
}