#pragma once

#include "light.hh"
#include "primitives/point.hh"

namespace scene
{
class SpotLight : public Light
{
  public:
    SpotLight(const primitives::Point3&  position,
              const primitives::Color&   color,
              const primitives::Vector3& direction,
              const double               angle);

    const primitives::Vector3& get_direction() const;

    double get_angle() const;

    const LightCaracteristics& get_caracteristics() const override;

    const primitives::Point3& get_center() const override;

    const primitives::Vector3
    get_ray_to_light(const primitives::Point3& point) const override;

  private:
    const primitives::Point3 position_;

    const LightCaracteristics caracteristics_;

    const primitives::Vector3 direction_;

    const double angle_;
};
} // namespace scene