#pragma once

#include "texture_material.hh"
#include "utils/perlin_noise.hh"

#include <vector>

namespace scene
{
enum PerlinTextureType
{
    DEFAULT = 0,
    MARBLE,
    WOOD,
};

class PerlinTexture : public TextureMaterial
{
  public:
    PerlinTexture(uint                                 octave,
                  double                               persistence,
                  const TextureMaterialCaracteristics& caracteristic,
                  PerlinTextureType                    type,
                  std::vector<primitives::Color>       color_list);

    TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const override;

  private:
    uint octave_;

    double persistence_;

    utils::PerlinNoise perlin_noise_;

    TextureMaterialCaracteristics caracteristic_;

    PerlinTextureType type_;

    std::vector<primitives::Color> color_list_;
};
} // namespace scene