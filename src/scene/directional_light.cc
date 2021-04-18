#include "directional_light.hh"

#include <limits>

namespace scene
{
DirectionalLight::DirectionalLight(const primitives::Color&   color,
                                   const primitives::Vector3& direction)
    : position_(std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity())
    , caracteristics_{color}
    , direction_(direction)
{
}

const primitives::Vector3& DirectionalLight::get_direction() const
{
    return direction_;
}

const LightCaracteristics& DirectionalLight::get_caracteristics() const
{
    return caracteristics_;
}

const primitives::Point3& DirectionalLight::get_center() const
{
    return position_;
}

const primitives::Vector3
DirectionalLight::get_ray_to_light(const primitives::Point3&) const
{
    return direction_.normalize() * -1.0;
}

bool DirectionalLight::is_illuminated(
    const primitives::Vector3& light_ray) const
{
    // A shadow is created if light_ray and get_direction() are in the same
    // orientation.
    return get_direction().dot(light_ray) < 0;
}
} // namespace scene