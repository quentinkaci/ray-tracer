#include "camera.hh"
#include "utils/transformation_matrix.hh"

#include <cmath>


namespace scene
{
    Camera::Camera(const primitives::Point3& center, const primitives::Point3& target,
        const primitives::Point3& up, float x_fov, float y_fov, float z_min)
        : center_(center)
        , target_(target)
        , up_(up)
        , x_fov_(x_fov)
        , y_fov_(y_fov)
        , z_min_(z_min)
    {}

    primitives::Vector3* Camera::get_pixels_vector(uint height, uint width) const
    {
        primitives::Vector3* res = new primitives::Vector3[height * width];

        float scale_x = tan(x_fov_ * 0.5 * M_PI / 180);
        float scale_y = tan(y_fov_ * 0.5 * M_PI / 180);
        float image_ratio = width / (float) height;

        utils::TransformationMatrix translation = utils::TransformationMatrix::translation_matrix(center_);
        utils::TransformationMatrix x_rotation = utils::TransformationMatrix::rotation_matrix(utils::RotationAxis::X);
        utils::TransformationMatrix y_rotation = utils::TransformationMatrix::rotation_matrix(utils::RotationAxis::Y);
        utils::TransformationMatrix z_rotation = utils::TransformationMatrix::rotation_matrix(utils::RotationAxis::Z,);

        utils::TransformationMatrix tranformation = z_rotation * y_rotation * x_rotation * translation;

        for (uint j = 0; j < height; ++j)
        {
            for (uint i = 0; i < width; ++i)
            {
                // x and y in [-1, 1] with pixel at (height / 2, width / 2) has position (0, 0)
                float x = (2 * (i + 0.5) / (float) width - 1) * image_ratio * scale_x;
                float y = - (2 * (j + 0.5) / (float) height - 1) * scale_y;

                primitives::Vector3 direction(primitives::Point3(x, y, z_min_));
                // TODO Apply transformation

                direction.normalize();
                res[i + j * width] = direction;
            }
        }

        return res;
    }
}