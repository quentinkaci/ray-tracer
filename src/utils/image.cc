#include "image.hh"

#include <fstream>
#include <cstdio>

namespace utils
{
    Image::Image(uint height, uint width)
        : height(height)
        , width(width)
    {
        pixels = new primitives::Color[width * height];
    }

    Image::~Image()
    {
        delete pixels;
    }

    primitives::Color& Image::pixel(const uint& x, const uint& y)
    {
        return pixels[x + y * width];
    }

    void Image::save_to_ppm(const std::string& filename)
    {
        std::ofstream ofs(filename + ".ppm", std::ios_base::out | std::ios_base::binary);
        ofs << "P6" << std::endl << width << ' ' << height << std::endl << "255" << std::endl;

        for (uint y = 0; y < height; ++y)
        {
            for (uint x = 0; x < width; ++x)
            {
                primitives::Color c = pixel(x, y);
                ofs << c.r << c.g << c.b << std::endl;
            }
        }

        ofs.close();
    }
}