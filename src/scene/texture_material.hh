#pragma once

#include "types.hh"
#include "primitives/color.hh"
#include "primitives/point.hh"


namespace scene
{
    struct TextureMaterialCaracteristics
    {
        uint kd;
        uint ks;
        primitives::Color color;
    };

    struct TextureMaterial
    {
        virtual const TextureMaterialCaracteristics& get_caracteristics(const primitives::Point3& point) const = 0;
    };
}