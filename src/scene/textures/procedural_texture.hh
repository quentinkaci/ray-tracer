#pragma once

#include "texture_material.hh"

namespace scene
{
class ProceduralTexture : public TextureMaterial
{
  public:
    ProceduralTexture(const TextureMaterialCaracteristics& caracteristic);

    TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const override;

  private:
    TextureMaterialCaracteristics caracteristic_;
};
} // namespace scene