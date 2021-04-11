#include "image_texture.hh"
#include "scene/objects/cube_aligned.hh"

#include <cmath>

namespace scene
{
ImageTexture::ImageTexture(const std::shared_ptr<utils::Image>& texture_image,
                           const TextureMaterialCaracteristics& caracteristic)
    : texture_image_(texture_image)
    , caracteristic_(caracteristic)
{
}

bool ImageTexture::get_cube_texture() const { return cube_texture_; }

void ImageTexture::set_cube_texture(bool val) { cube_texture_ = val; }

TextureMaterialCaracteristics
ImageTexture::get_caracteristics(const primitives::Point3& point) const
{
    TextureMaterialCaracteristics caracteristic(caracteristic_);

    // Handle 6 faces of a cube
    if (cube_texture_)
    {
        CUBE_FACES face = static_cast<CUBE_FACES>(point.z);

        double x_step = 1.0 / 4.0;
        double y_step = 1.0 / 3.0;

        double x = 0;
        double y = 0;

        if (face == CUBE_FACES::FRONT)
        {
            x = 2 * x_step + point.x / 4;
            y = 1 * y_step + point.y / 3;
        }
        else if (face == CUBE_FACES::BACK)
        {
            x = 0 * x_step + point.x / 4;
            y = 1 * y_step + point.y / 3;
        }
        else if (face == CUBE_FACES::TOP)
        {
            x = 2 * x_step + point.x / 4;
            y = 2 * y_step + point.y / 3;
        }
        else if (face == CUBE_FACES::DOWN)
        {
            x = 2 * x_step + point.x / 4;
            y = 0 * y_step + point.y / 3;
        }
        else if (face == CUBE_FACES::LEFT)
        {
            x = 1 * x_step + point.x / 4;
            y = 1 * y_step + point.y / 3;
        }
        else if (face == CUBE_FACES::RIGHT)
        {
            x = 3 * x_step + point.x / 4;
            y = 1 * y_step + point.y / 3;
        }

        caracteristic.color = texture_image_->get_pixel(
            x * texture_image_->width, (1 - y) * texture_image_->height);
        return caracteristic;
    }

    caracteristic.color = texture_image_->get_pixel(
        point.x * texture_image_->width, point.y * texture_image_->height);
    return caracteristic;
}
} // namespace scene