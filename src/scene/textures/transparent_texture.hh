#pragma once

#include "texture_material.hh"

namespace scene
{
class TransparentTexture : public TextureMaterial
{
  public:
    TransparentTexture(const TextureMaterialCaracteristics& caracteristic);

    TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const override;

    primitives::Vector3
    get_scattered_ray(const primitives::Vector3& ray,
                      const primitives::Vector3& normal) const override;

  private:
    TextureMaterialCaracteristics caracteristic_;
};
} // namespace scene