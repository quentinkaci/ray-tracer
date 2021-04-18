#pragma once

#include "light.hh"
#include "primitives/point.hh"

namespace scene
{
class DirectionalLight : public Light
{
  public:
    DirectionalLight(const primitives::Color&   color,
                     const primitives::Vector3& direction);

    const primitives::Vector3& get_direction() const;

    const LightCaracteristics& get_caracteristics() const override;

    const primitives::Point3& get_center() const override;

    const primitives::Vector3
    get_ray_to_light(const primitives::Point3& point) const override;

    bool is_illuminated(const primitives::Vector3& light_ray) const override;

  private:
    const primitives::Point3 position_;

    const LightCaracteristics caracteristics_;

    const primitives::Vector3 direction_;
};
} // namespace scene