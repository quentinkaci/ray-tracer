#pragma once

#include "primitives/vector.hh"


namespace scene
{
    class Camera
    {
    public:
        // All angles are supposed to be in degree

        Camera(const primitives::Point3& origin, const primitives::Point3& target,
            const primitives::Point3& up, double x_fov, double y_fov, double z_min);

        primitives::Point3 get_origin() const;

        primitives::Vector3* get_pixels_vector(uint height, uint width) const;

    private:
        primitives::Point3 origin_;

        primitives::Point3 target_;

        primitives::Vector3 up_;

        double x_fov_;

        double y_fov_;

        double z_min_;
    };
}