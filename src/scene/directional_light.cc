#include "directional_light.hh"

namespace scene
{
DirectionalLight::DirectionalLight(const primitives::Color&   color,
                                   const primitives::Vector3& direction)
    : position_()
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
} // namespace scene