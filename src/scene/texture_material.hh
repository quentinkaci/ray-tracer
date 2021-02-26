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
        virtual const TextureMaterialCaracteristics& get_texture(const primitives::Point3& point) = 0;
    };
}