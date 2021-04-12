#pragma once

#include "primitives/color.hh"
#include "types.hh"

#include <memory>
#include <vector>

namespace utils
{
class Image
{
  public:
    explicit Image();

    virtual ~Image();

    primitives::Color& pixel(const uint& i, const uint& j);

    primitives::Color get_pixel(const uint& x, const uint& y) const;

    uint get_width() const;

    uint get_height() const;

    void create(const uint width, const uint height);

    virtual void save(const std::string& filename);
    virtual void load(const std::string& filename);

  protected:
    uint                           height_;
    uint                           width_;
    std::vector<primitives::Color> pixels_;
};

std::shared_ptr<Image> create_image(const std::string& filename);
} // namespace utils