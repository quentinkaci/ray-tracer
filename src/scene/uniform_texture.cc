#include "uniform_texture.hh"

namespace scene
{
    UniformTexture::UniformTexture(Caracteristic caracteristic)
        : caracteristic_(caracteristic)
    {}

    Caracteristic UniformTexture::get_texture(const uint& x, const uint& y)
    {
        return caracteristic_;
    }
}