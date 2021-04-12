#include "spot_light.hh"

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

} // namespace scene