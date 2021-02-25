#pragma once

#include "types.hh"
#include "primitives/color.hh"

#include <memory>

namespace utils
{
    struct Image
    {
        Image(uint height, uint width);

        ~Image();

        primitives::Color& pixel(const uint& i, const uint& j);

        void save_to_ppm(const std::string& filename);

        uint height;
        uint width;
        primitives::Color* pixels = nullptr;
    };
}