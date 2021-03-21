#pragma once

#include "primitives/color.hh"
#include "primitives/point.hh"
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
};
} // namespace scene