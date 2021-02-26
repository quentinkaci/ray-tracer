#pragma once

#include "light.hh"
#include "primitives/point.hh"


namespace scene
{
    class PointLight : Light
    {
    public:
        PointLight(const primitives::Point3& position, const primitives::Color& color, uint intensity);

        const LightCaracteristics& get_caracteristics() const override;

        const primitives::Point3& get_center() const override;

    private:
        const primitives::Point3& position_;

        LightCaracteristics caracteristics_;
    };
}