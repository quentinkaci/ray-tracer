#pragma once

#include "texture_material.hh"


namespace scene
{
    class UniformTexture : TextureMaterial
    {
    public:
        UniformTexture(Caracteristic caracteristic);

        Caracteristic get_texture(const uint& x, const uint& y) override;

    private:
        Caracteristic caracteristic_;
    };
}