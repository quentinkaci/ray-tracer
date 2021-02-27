#include "transformation_matrix.hh"

#include <cmath>


namespace utils
{
    TransformationMatrix::TransformationMatrix()
    {
        matrix_ = new double[16]{
                    1., 0., 0., 0.,
                    0., 1., 0., 0.,
                    0., 0., 1., 0.,
                    0., 0., 0., 1.};
    }

    TransformationMatrix TransformationMatrix::translation_matrix(const primitives::Point3& point)
    {
        TransformationMatrix res;
        res.matrix_ = new double[16]{
                    1., 0., 0., - point.x,
                    0., 1., 0., - point.y,
                    0., 0., 1., - point.z,
                    0., 0., 0., 1.};
        return res;
    }

    TransformationMatrix TransformationMatrix::rotation_matrix(RotationAxis axis, float angle)
    {
        TransformationMatrix res;

        switch (axis)
        {
        case RotationAxis::X:
            res.matrix_ = new double[16]{
                    cos(angle), - sin(angle), 0., 0.,
                    sin(angle), cos(angle), 0., 0.,
                    0., 0., 1., 0.,
                    0., 0., 0., 1.};
            break;
        case RotationAxis::Y:
            res.matrix_ = new double[16]{
                    cos(angle), 0., sin(angle), 0.,
                    0., 1., 0., 0.,
                    - sin(angle), 0., cos(angle), 0.,
                    0., 0., 0., 1.};
            break;
        case RotationAxis::Z:
            // TODO
            break;
        default:
            break;
        }

        return res;
    }

    TransformationMatrix TransformationMatrix::operator*(const TransformationMatrix& matrix) const
    {
        TransformationMatrix res;

        for (uint i = 0; i < 4; ++i)
        {
            for (uint j = 0; j < 4; ++j)
            {
                for (uint k = 0; k < 4; ++k)
                    res.matrix_[i + j * 4] += matrix_[i + k * 4] * matrix.matrix_[k + j * 4];
            }
        }

        return res;
    }

    primitives::Vector3 TransformationMatrix::operator*(const primitives::Vector3& vector) const
    {
        int x = matrix_[0] * vector.dst.x + matrix_[1] * vector.dst.y
                + matrix_[2] * vector.dst.z + matrix_[3];
        int y = matrix_[4] * vector.dst.x + matrix_[5] * vector.dst.y
                + matrix_[6] * vector.dst.z + matrix_[7];
        int z = matrix_[8] * vector.dst.x + matrix_[9] * vector.dst.y
                + matrix_[10] * vector.dst.z + matrix_[11];

        return primitives::Vector3(primitives::Point3(x, y, z));
    }
}