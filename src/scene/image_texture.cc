#include "image_texture.hh"

#include <cmath>


namespace scene
{
    ImageTexture::ImageTexture(const utils::Image& texture_image, const TextureMaterialCaracteristics& caracteristic)
        : texture_image_(texture_image)
        , caracteristic_(caracteristic)
    {}

    TextureMaterialCaracteristics ImageTexture::get_caracteristics(const primitives::Point3& point) const
    {
        TextureMaterialCaracteristics caracteristic(caracteristic_);
        caracteristic.color = texture_image_.get_pixel(point.x * texture_image_.width, point.y * texture_image_.height);
        return caracteristic;
    }
}