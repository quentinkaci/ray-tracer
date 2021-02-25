#pragma once

#include "types.hh"
#include "color.hh"


namespace scene
{
    struct Caracteristic
    {
        uint kd;
        uint ks;
        primitives::Color color;
    };

    struct TextureMaterial
    {
        virtual Caracteristic get_texture(const uint& x, const uint& y) = 0;
    };
}