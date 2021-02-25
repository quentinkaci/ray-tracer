#include "point.hh"

namespace primitives
{
    Point3::Point3(int x, int y, int z)
        : x(x)
        , y(y)
        , z(z)
    {}

    Point3 Point3::operator*(const float& n) const
    {
        return Point3(x * n, y * n, z * n);
    }

    Point3 Point3::operator-(const Point3& p) const
    {
        return Point3(x - p.x, y - p.y, z - p.z);
    }
}


std::ostream& operator<<(std::ostream& os, const primitives::Point3& point)
{
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
    return os;
}