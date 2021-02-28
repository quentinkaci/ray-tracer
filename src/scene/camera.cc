#include "camera.hh"

#include <cmath>


namespace scene
{
    Camera::Camera(const primitives::Point3& origin, const primitives::Point3& target,
        const primitives::Point3& up, double x_fov, double y_fov, double z_min)
        : origin_(origin)
        , target_(target)
        , up_(up)
        , x_fov_(x_fov)
        , y_fov_(y_fov)
        , z_min_(z_min)
    {}

    primitives::Point3 Camera::get_origin() const
    {
        return origin_;
    }

    primitives::Vector3* Camera::get_pixels_vector(uint height, uint width) const
    {
        primitives::Vector3* res = new primitives::Vector3[height * width];

        primitives::Vector3 forward(target_ - origin_);
        primitives::Vector3 right = forward.cross(up_);

        double image_plane_width = 2 * z_min_ * tan(x_fov_ * 0.5 * M_PI / 180.0);
        double image_plane_height = 2 * z_min_ * tan(y_fov_ * 0.5 * M_PI / 180.0);

        primitives::Vector3 up_left = forward * z_min_ + up_ * (image_plane_height / 2) - right * (image_plane_width / 2);

        double image_plane_ratio_width = image_plane_width / width;
        double image_plane_ratio_height = image_plane_height / height;

        for (uint j = 0; j < height; ++j)
        {
            for (uint i = 0; i < width; ++i)
            {
                primitives::Vector3 v = up_left + right * image_plane_ratio_width * i
                                        - up_ * image_plane_ratio_height * j;
                v.normalize();
                res[i + j * width] = v;
            }
        }

        return res;
    }
}