#pragma once

#include "types.hh"

#include <iostream>


namespace primitives
{
    struct Point3
    {
        Point3(int x, int y, int z);

        Point3 operator*(const float& n) const;

        Point3 operator-(const Point3& p) const;

        int x;
        int y;
        int z;
    };
}


std::ostream& operator<<(std::ostream& os, const primitives::Point3& point);