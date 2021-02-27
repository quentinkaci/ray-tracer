#include "uniform_texture.hh"

namespace scene
{
    UniformTexture::UniformTexture(const TextureMaterialCaracteristics& caracteristic)
        : caracteristic_(caracteristic)
    {}

    const TextureMaterialCaracteristics& UniformTexture::get_caracteristics(const primitives::Point3&) const
    {
        return caracteristic_;
    }
}