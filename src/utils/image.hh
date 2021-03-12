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

        // Only load P3 ppm files
        static Image* load_from_ppm(const std::string& filename);

        primitives::Color& pixel(const uint& i, const uint& j);

        primitives::Color get_pixel(const uint& x, const uint& y) const;

        void save_to_ppm(const std::string& filename);

        uint height;
        uint width;
        primitives::Color* pixels = nullptr;
    };
}