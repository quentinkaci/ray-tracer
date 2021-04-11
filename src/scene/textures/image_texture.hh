#pragma once

#include "texture_material.hh"
#include "utils/image.hh"

namespace scene
{
class ImageTexture : public TextureMaterial
{
  public:
    // FIXME Maybe take a structure TextureMap that links pixels to a
    // TextureMaterialCaracteristics
    ImageTexture(const std::shared_ptr<utils::Image>& texture_image,
                 const TextureMaterialCaracteristics& caracteristic);

    TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const override;

    bool get_cube_texture() const;
    void set_cube_texture(bool val);

  private:
    const std::shared_ptr<utils::Image> texture_image_;

    TextureMaterialCaracteristics caracteristic_;

    bool cube_texture_ = false;
};
} // namespace scene