#pragma once

#include "texture_material.hh"


namespace scene
{
    class UniformTexture : TextureMaterial
    {
    public:
        UniformTexture(Caracteristic caracteristic);

        Caracteristic get_texture(const uint&, const uint&) override;

    private:
        Caracteristic caracteristic_;
    };
}