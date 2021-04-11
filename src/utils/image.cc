#include "image.hh"

#include <cstdio>
#include <fstream>

namespace utils
{
Image::Image(uint height, uint width)
    : height(height)
    , width(width)
{
    pixels = new primitives::Color[width * height];
}

Image::~Image() { delete[] pixels; }

std::shared_ptr<Image> Image::load_from_ppm(const std::string& filename)
{
    std::ifstream ifs(filename + ".ppm", std::ios_base::out);
    if (!ifs.is_open())
        return nullptr;

    std::string buffer;

    ifs >> buffer;
    if (buffer != "P3")
    {
        std::cerr << "Error: PPM format not handled." << std::endl;
        return nullptr;
    }

    ifs >> buffer;
    uint width = std::stoul(buffer);
    ifs >> buffer;
    uint height = std::stoul(buffer);

    // Skip largest value of RGB
    ifs >> buffer;

    auto res = std::make_shared<Image>(height, width);

    for (uint y = 0; y < height; ++y)
    {
        for (uint x = 0; x < width; ++x)
        {
            ifs >> buffer;
            uchar r = std::stoul(buffer);
            ifs >> buffer;
            uchar g = std::stoul(buffer);
            ifs >> buffer;
            uchar b = std::stoul(buffer);

            res->pixel(x, y) = primitives::Color(r, g, b);
        }
    }

    ifs.close();

    return res;
}

primitives::Color& Image::pixel(const uint& x, const uint& y)
{
    return pixels[x + y * width];
}

primitives::Color Image::get_pixel(const uint& x, const uint& y) const
{
    return pixels[x + y * width];
}

void Image::save_to_ppm(const std::string& filename)
{
    std::ofstream ofs(filename + ".ppm", std::ios_base::out);
    ofs << "P3" << std::endl
        << width << " " << height << std::endl
        << "255" << std::endl;

    for (uint y = 0; y < height; ++y)
    {
        for (uint x = 0; x < width; ++x)
        {
            primitives::Color c = pixel(x, y);
            ofs << " " << std::to_string(c.r) << " " << std::to_string(c.g)
                << " " << std::to_string(c.b) << " ";
        }
        ofs << std::endl;
    }

    ofs.close();
}
} // namespace utils