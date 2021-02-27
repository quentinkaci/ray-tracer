#include "camera.hh"
#include "utils/transformation_matrix.hh"

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

        double scale_x = tan(x_fov_ * 0.5 * M_PI / 180);
        double scale_y = tan(y_fov_ * 0.5 * M_PI / 180);
        double image_ratio = width / (double) height;

        utils::TransformationMatrix translation = utils::TransformationMatrix::translation_matrix(origin_);

        // FIXME Calculate correct angles and check axis
        utils::TransformationMatrix x_rotation = utils::TransformationMatrix::rotation_matrix(utils::RotationAxis::X, 0);
        utils::TransformationMatrix y_rotation = utils::TransformationMatrix::rotation_matrix(utils::RotationAxis::Y, 0);
        utils::TransformationMatrix z_rotation = utils::TransformationMatrix::rotation_matrix(utils::RotationAxis::Z, 0);

        utils::TransformationMatrix tranformation = z_rotation * y_rotation * x_rotation * translation;

        for (uint j = 0; j < height; ++j)
        {
            for (uint i = 0; i < width; ++i)
            {
                // x and y in [-1, 1] with pixel at (height / 2, width / 2) has position (0, 0)
                double x = (2 * (i + 0.5) / (double) width - 1) * image_ratio * scale_x;
                double y = - (2 * (j + 0.5) / (double) height - 1) * scale_y;

                primitives::Vector3 direction = tranformation.apply(primitives::Vector3(primitives::Point3(x, y, z_min_)));
                direction.normalize();
                res[i + j * width] = direction;
            }
        }

        return res;
    }
}