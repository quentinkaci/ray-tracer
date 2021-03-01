#pragma once

#include "primitives/color.hh"
#include "primitives/vector.hh"

#include <vector>


namespace scene
{
    struct LightCaracteristics
    {
        primitives::Color color;
    };

    struct Light
    {
        virtual const LightCaracteristics& get_caracteristics() const = 0;

        virtual const primitives::Point3& get_center() const = 0;
    };
}