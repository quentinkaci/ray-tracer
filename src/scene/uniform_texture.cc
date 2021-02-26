#include "uniform_texture.hh"

namespace scene
{
    UniformTexture::UniformTexture(const TextureMaterialCaracteristics& caracteristic)
        : caracteristic_(caracteristic)
    {}

    const TextureMaterialCaracteristics& UniformTexture::get_texture(const primitives::Point3&)
    {
        return caracteristic_;
    }
}