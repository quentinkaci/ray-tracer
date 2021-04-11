#pragma once

#include "texture_material.hh"

namespace scene
{
class ProceduralTexture : public TextureMaterial
{
  public:
    ProceduralTexture(const std::string&                   style,
                      const double                         size,
                      const TextureMaterialCaracteristics& caracteristic);

    TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const override;

  private:
    const std::string             style_;
    const double                  size_;
    TextureMaterialCaracteristics caracteristic_;
};
} // namespace scene