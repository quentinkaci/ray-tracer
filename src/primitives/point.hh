#pragma once

#include "types.hh"

#include <iostream>


namespace primitives
{
    struct Point3
    {
        Point3(double x, double y, double z);

        Point3();

        Point3 operator*(const double& n) const;

        Point3 operator/(const double& n) const;

        Point3 operator+(const Point3& p) const;

        Point3 operator-(const Point3& p) const;

        double x;
        double y;
        double z;
    };
}


std::ostream& operator<<(std::ostream& os, const primitives::Point3& point);