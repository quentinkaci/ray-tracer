#pragma once

#include "primitives/vector.hh"


namespace scene
{
    class Camera
    {
    public:
        // All angles are supposed to be in degree

        Camera(const primitives::Point3& center, const primitives::Point3& target,
            const primitives::Point3& up, float x_fov, float y_fov, float z_min);

        primitives::Vector3* get_pixels_vector(uint height, uint width) const;

    private:
        primitives::Point3 center_;

        primitives::Point3 target_;

        primitives::Vector3 up_;

        float x_fov_;

        float y_fov_;

        float z_min_;
    };
}