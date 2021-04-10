#include "uniform_texture.hh"

namespace scene
{
UniformTexture::UniformTexture(
    const TextureMaterialCaracteristics& caracteristic)
    : caracteristic_(caracteristic)
{
}

TextureMaterialCaracteristics
UniformTexture::get_caracteristics(const primitives::Point3&) const
{
    return caracteristic_;
}
} // namespace scene