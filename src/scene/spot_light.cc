#include "spot_light.hh"

#include <cmath>

namespace scene
{
SpotLight::SpotLight(const primitives::Point3&  position,
                     const primitives::Color&   color,
                     const primitives::Vector3& direction,
                     const double               angle)
    : position_(position)
    , caracteristics_{color}
    , direction_(direction)
    , angle_(angle)
{
}

const primitives::Vector3& SpotLight::get_direction() const
{
    return direction_;
}

double SpotLight::get_angle() const { return angle_; }

const LightCaracteristics& SpotLight::get_caracteristics() const
{
    return caracteristics_;
}

const primitives::Point3& SpotLight::get_center() const { return position_; }

const primitives::Vector3
SpotLight::get_ray_to_light(const primitives::Point3& point) const
{
    primitives::Vector3 ray(get_center() - point);
    ray = ray.normalize();
    return ray;
}

bool SpotLight::is_illuminated(const primitives::Vector3& light_ray) const
{
    // A shadow is created if the angle between light_ray and
    // get_direction() is greater than a specific value.

    // Rays must be in the same orientation
    const auto light_ray_inv = light_ray * -1.0;

    const double theta =
        std::acos(get_direction().dot(light_ray_inv) /
                  (get_direction().norm() * light_ray_inv.norm()));

    return std::fabs(theta) <= get_angle();
}

} // namespace scene