#include "image.hh"

#include <cstdio>
#include <fstream>

static void safe_read(std::ifstream& ifs, std::string& buffer)
{
    ifs >> buffer;
    if (ifs.fail() || ifs.eof())
        throw std::logic_error("PPM file is corrupted.");
}

namespace utils
{
class ImagePPM : public Image
{
  public:
    void load(const std::string& filename)
    {
        std::ifstream ifs(filename, std::ios_base::out);
        if (!ifs.is_open())
            throw std::logic_error("PPM file (" + filename +
                                   ") doesn't exist.");

        std::string buffer;

        safe_read(ifs, buffer);
        if (buffer != "P3")
            throw std::logic_error("PPM format not handled.");

        safe_read(ifs, buffer);
        uint width = std::stoul(buffer);
        safe_read(ifs, buffer);
        uint height = std::stoul(buffer);

        safe_read(ifs, buffer);
        if (std::stoul(buffer) != 255)
            throw std::logic_error("RGB depth must be 8 bit.");

        height_ = height;
        width_  = width;
        pixels_ = std::vector<primitives::Color>(width * height);

        for (uint y = 0; y < height; ++y)
        {
            for (uint x = 0; x < width; ++x)
            {
                safe_read(ifs, buffer);

                uchar r = std::stoul(buffer);
                safe_read(ifs, buffer);
                uchar g = std::stoul(buffer);
                safe_read(ifs, buffer);
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