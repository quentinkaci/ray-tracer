#include "point_light.hh"

namespace scene
{
    PointLight::PointLight(const primitives::Point3& position, const primitives::Color& color, uint intensity)
        : position_(position)
        , caracteristics_{intensity, color}
    {}

    const LightCaracteristics& PointLight::get_caracteristics() const
    {
        return caracteristics_;
    }

    const primitives::Point3& PointLight::get_center() const
    {
        return position_;
    }
}