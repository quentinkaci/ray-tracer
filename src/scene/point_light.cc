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
} // namespace scene