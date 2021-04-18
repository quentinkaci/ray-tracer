#pragma once

#include "light.hh"
#include "primitives/point.hh"

namespace scene
{
class PointLight : public Light
{
  public:
    PointLight(const primitives::Point3& position,
               const primitives::Color&  color);

    const LightCaracteristics& get_caracteristics() const override;

    const primitives::Point3& get_center() const override;

    const primitives::Vector3
    get_ray_to_light(const primitives::Point3& point) const override;

    bool is_illuminated(const primitives::Vector3& light_ray) const override;

  private:
    const primitives::Point3 position_;

    const LightCaracteristics caracteristics_;
};
} // namespace scene