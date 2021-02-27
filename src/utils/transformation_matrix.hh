#pragma once

#include "primitives/vector.hh"


namespace utils
{
    // Camera coordinates (X: Horizontal, Y: Vertical, Z: Depth)
    enum RotationAxis
    {
        X = 0,
        Y,
        Z
    };

    class TransformationMatrix
    {
        // 4 x 4 matrix used to apply tranformations

    public:
        TransformationMatrix();

        static TransformationMatrix translation_matrix(const primitives::Point3& point);

        // Angle in radian
        static TransformationMatrix rotation_matrix(RotationAxis axis, float angle);

        TransformationMatrix operator*(const TransformationMatrix& matrix) const;

        primitives::Vector3 operator*(const primitives::Vector3& vector) const;

    private:
        double* matrix_;
    };
}