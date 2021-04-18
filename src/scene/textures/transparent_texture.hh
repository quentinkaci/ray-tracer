#pragma once

#include "texture_material.hh"

namespace scene
{
class TransparentTexture : public TextureMaterial
{
  public:
    TransparentTexture(const double                         refractive_index,
                       const TextureMaterialCaracteristics& caracteristic);

    TextureMaterialCaracteristics
    get_caracteristics(const primitives::Point3& point) const override;

    primitives::Vector3
    get_scattered_ray(const primitives::Vector3& ray,
                      const primitives::Vector3& normal,
                      SCATTERED_RAY& scattered_ray_type) const override;

  private:
    bool schlick_approximation(const double n_1,
                               const double n_2,
                               const double cos_theta_1) const;

    double refractive_index_;

    TextureMaterialCaracteristics caracteristic_;
};
} // namespace scene