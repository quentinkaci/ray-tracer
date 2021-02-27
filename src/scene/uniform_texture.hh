#pragma once

#include "texture_material.hh"


namespace scene
{
    class UniformTexture : public TextureMaterial
    {
    public:
        UniformTexture(const TextureMaterialCaracteristics& caracteristic);

        const TextureMaterialCaracteristics& get_caracteristics(const primitives::Point3& point) const override;

    private:
        TextureMaterialCaracteristics caracteristic_;
    };
}