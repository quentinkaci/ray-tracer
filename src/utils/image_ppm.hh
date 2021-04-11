#include "image.hh"

#include <cstdio>
#include <fstream>

namespace utils
{
class ImagePPM : public Image
{
  public:
    void load(const std::string& filename)
    {
        std::ifstream ifs(filename, std::ios_base::out);
        if (!ifs.is_open())
            return;

        std::string buffer;

        ifs >> buffer;
        if (buffer != "P3")
        {
            std::cerr << "Error: PPM format not handled." << std::endl;
            return;
        }

        ifs >> buffer;
        uint width = std::stoul(buffer);
        ifs >> buffer;
        uint height = std::stoul(buffer);

        // Skip largest value of RGB
        ifs >> buffer;

        height_ = height;
        width_  = width;
        pixels_ = new primitives::Color[width * height];

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

                pixel(x, y) = primitives::Color(r, g, b);
            }
        }

        ifs.close();
    }

    void save(const std::string& filename)
    {
        std::ofstream ofs(filename, std::ios_base::out);
        ofs << "P3" << std::endl
            << width_ << " " << height_ << std::endl
            << "255" << std::endl;

        for (uint y = 0; y < height_; ++y)
        {
            for (uint x = 0; x < width_; ++x)
            {
                primitives::Color c = pixel(x, y);
                ofs << " " << std::to_string(c.r) << " " << std::to_string(c.g)
                    << " " << std::to_string(c.b) << " ";
            }
            ofs << std::endl;
        }

        ofs.close();
    }
};

} // namespace utils