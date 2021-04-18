#include "image.hh"

#include <png++/png.hpp>

namespace utils
{
class ImagePNG : public Image
{
  public:
    void load(const std::string& filename)
    {
        png::image<png::rgb_pixel> image(filename);

        width_  = image.get_width();
        height_ = image.get_height();
        pixels_ = std::vector<primitives::Color>(width_ * height_);

        for (size_t j = 0; j < image.get_height(); ++j)
        {
            for (size_t i = 0; i < image.get_width(); ++i)
            {
                auto pix    = image[j][i];
                pixel(i, j) = primitives::Color(pix.red, pix.green, pix.blue);
            }
        }
    }

    void save(const std::string& filename)
    {
        png::image<png::rgb_pixel> image(width_, height_);

        for (size_t j = 0; j < height_; ++j)
        {
            for (size_t i = 0; i < width_; ++i)
            {
                auto pix    = pixel(i, j);
                image[j][i] = png::rgb_pixel(pix.r, pix.g, pix.b);
            }
        }

        image.write(filename);
    }
};

} // namespace utils