#pragma once

#include "types.hh"

#include <iostream>

namespace primitives
{
struct Point3
{
    Point3(double x, double y, double z);

    Point3();

    double x;
    double y;
    double z;
};

Point3 operator*(const Point3& point, const double& n);

Point3 operator/(const Point3& point, const double& n);

Point3 operator+(const Point3& left, const Point3& right);

Point3 operator-(const Point3& left, const Point3& right);

std::ostream& operator<<(std::ostream& os, const Point3& point);
} // namespace primitives