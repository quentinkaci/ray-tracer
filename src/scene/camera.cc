#include "camera.hh"

#include <cmath>

namespace scene
{
Camera::Camera(const primitives::Point3& origin,
               const primitives::Point3& target,
               const primitives::Point3& up,
               double                    x_fov,
               double                    y_fov,
               double                    z_min)
    : origin_(origin)
    , target_(target)
    , up_(up)
    , x_fov_(x_fov)
    , y_fov_(y_fov)
    , z_min_(z_min)
{
}

primitives::Point3 Camera::get_origin() const { return origin_; }

primitives::Point3 Camera::get_target() const { return target_; }

double Camera::get_unit_x(uint width) const
{
    double image_plane_width = 2 * z_min_ * tan(x_fov_ * 0.5 * M_PI / 180.0);
    return image_plane_width / width;
}

double Camera::get_unit_y(uint height) const
{
    double image_plane_height = 2 * z_min_ * tan(y_fov_ * 0.5 * M_PI / 180.0);
    return image_plane_height / height;
}

std::vector<primitives::Vector3> Camera::get_pixels_vector(uint height,
                                                           uint width) const
{
    std::vector<primitives::Vector3> res(height * width);

    primitives::Vector3 forward =
        primitives::Vector3(target_ - origin_).normalize();
    primitives::Vector3 right = forward.cross(up_).normalize();
    primitives::Vector3 up    = up_.normalize();

    double image_plane_width  = 2 * z_min_ * tan(x_fov_ * 0.5 * M_PI / 180.0);
    double image_plane_height = 2 * z_min_ * tan(y_fov_ * 0.5 * M_PI / 180.0);

    primitives::Vector3 top_left = forward * z_min_ +
                                   up * (image_plane_height / 2) -
                                   right * (image_plane_width / 2);

    double unit_x = image_plane_width / width;
    double unit_y = image_plane_height / height;

    for (uint j = 0; j < height; ++j)
    {
        for (uint i = 0; i < width; ++i)
        {
            primitives::Vector3 v =
                top_left + right * unit_x * i - up * unit_y * j;
            res[i + j * width] = v.normalize();
        }
    }

    return res;
}

void Camera::translate(const primitives::Point3& translation)
{
    origin_ = origin_ + translation;
}
} // namespace scene