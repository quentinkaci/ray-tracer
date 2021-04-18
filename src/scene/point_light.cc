#include "point_light.hh"

namespace scene
{
PointLight::PointLight(const primitives::Point3& position,
                       const primitives::Color&  color)
    : position_(position)
    , caracteristics_{color}
{
}

const LightCaracteristics& PointLight::get_caracteristics() const
{
    return caracteristics_;
}

const primitives::Point3& PointLight::get_center() const { return position_; }

const primitives::Vector3
PointLight::get_ray_to_light(const primitives::Point3& point) const
{
    primitives::Vector3 ray(get_center() - point);
    ray = ray.normalize();
    return ray;
}

bool PointLight::is_illuminated(const primitives::Vector3&) const
{
    return true;
}

} // namespace scene