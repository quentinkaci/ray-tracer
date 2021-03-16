#pragma once

#include "texture_material.hh"
#include "primitives/vector.hh"
#include "utils/perlin_noise.hh"


namespace scene
{
    class BumpMappingTexture : public TextureMaterial
    {
    public:
        BumpMappingTexture(uint octave, double persistence, double instensity, const TextureMaterialCaracteristics& caracteristic);

        TextureMaterialCaracteristics get_caracteristics(const primitives::Point3& point) const override;

        primitives::Vector3 get_normal_perturbation(const primitives::Point3& point) const;

    private:
        utils::PerlinNoise p_;

        uint octave_;

        double persistence_;

        double intensity_;

        TextureMaterialCaracteristics caracteristic_;
    };
}