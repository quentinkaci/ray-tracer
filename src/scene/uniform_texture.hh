#pragma once

#include "texture_material.hh"


namespace scene
{
    class UniformTexture : TextureMaterial
    {
    public:
        UniformTexture(const TextureMaterialCaracteristics& caracteristic);

        const TextureMaterialCaracteristics& get_texture(const primitives::Point3& point) override;

    private:
        TextureMaterialCaracteristics caracteristic_;
    };
}