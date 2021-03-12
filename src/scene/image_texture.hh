#pragma once

#include "texture_material.hh"
#include "utils/image.hh"


namespace scene
{
    class ImageTexture : public TextureMaterial
    {
    public:
        // FIXME Maybe take a structure TextureMap that links pixels to a TextureMaterialCaracteristics
        ImageTexture(const utils::Image& texture_image, const TextureMaterialCaracteristics& caracteristic);

        TextureMaterialCaracteristics get_caracteristics(const primitives::Point3& point) const override;

    private:
        const utils::Image& texture_image_;
        TextureMaterialCaracteristics caracteristic_;
    };
}