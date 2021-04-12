#include "image.hh"

#include "image_png.hh"
#include "image_ppm.hh"

namespace utils
{
Image::Image() {}

Image::~Image() {}

primitives::Color& Image::pixel(const uint& x, const uint& y)
{
    return pixels_[x + y * width_];
}

primitives::Color Image::get_pixel(const uint& x, const uint& y) const
{
    return pixels_[x + y * width_];
}

uint Image::get_width() const { return width_; }

uint Image::get_height() const { return height_; }

void Image::create(const uint width, const uint height)
{
    height_ = height;
    width_  = width;
    pixels_ = std::vector<primitives::Color>(width * height);
}

void Image::save(const std::string&) {}

void Image::load(const std::string&) {}

std::shared_ptr<Image> create_image(const std::string& filename)
{
    auto ext = filename.substr(filename.size() - 4);
    if (ext == ".ppm")
        return std::make_shared<ImagePPM>();
    else if (ext == ".png")
        return std::make_shared<ImagePNG>();
    return nullptr;
}
} // namespace utils