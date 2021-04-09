#pragma once

#include "primitives/color.hh"
#include "primitives/point.hh"
#include "primitives/vector.hh"
#include "types.hh"

namespace scene
{
struct TextureMaterialCaracteristics
{
    double            kd;
    double            ks;
    double            ns;
    double            reflection;
    primitives::Color color;
};

struct TextureMaterial
{
    virtual TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const = 0;

    virtual primitives::Vector3
    get_scattered_ray(const primitives::Vector3& ray,
                      const primitives::Vector3& normal) const
    {
        // By default the scattered ray is the reflected ray.
        return (ray - normal * ray.dot(normal) * 2).normalize();
    }
};
} // namespace scene