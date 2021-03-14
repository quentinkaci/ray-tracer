#pragma once

#include "point.hh"

#include <iostream>

namespace primitives
{
    struct Vector3
    {
        Vector3(double x, double y, double z);

        Vector3(const Point3& point);

        Vector3();

        double dot(const Vector3& v) const;

        Vector3 cross(const Vector3& v) const;

        double norm() const;

        Vector3 normalize() const;

        Point3 get_destination() const;

        double x;
        double y;
        double z;
    };

    Vector3 operator*(const Vector3& v, const double& n);

    Vector3 operator/(const Vector3& v, const double& n);

    Vector3 operator*(const Vector3& left, const Vector3& right);

    Vector3 operator/(const Vector3& left, const Vector3& right);

    Vector3 operator+(const Vector3& left, const Vector3& right);

    Vector3 operator-(const Vector3& left, const Vector3& right);

    std::ostream& operator<<(std::ostream& os, const Vector3& v);
}