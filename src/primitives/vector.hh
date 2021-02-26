#pragma once

#include "types.hh"
#include "point.hh"

#include <iostream>

namespace primitives
{
    struct Vector3
    {
        Vector3(Point3 dst);

        Vector3 operator*(const float& n) const;

        Vector3 operator-(const Vector3& v) const;

        Point3 dst;
    };
}


std::ostream& operator<<(std::ostream& os, const primitives::Vector3& color);