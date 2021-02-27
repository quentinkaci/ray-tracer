#include "point.hh"

namespace primitives
{
    Point3::Point3(double x, double y, double z)
        : x(x)
        , y(y)
        , z(z)
    {}

    Point3::Point3()
        : Point3(0, 0, 0)
    {}

    Point3 Point3::operator*(const double& n) const
    {
        return Point3(x * n, y * n, z * n);
    }

    Point3 Point3::operator/(const double& n) const
    {
        return Point3(x / n, y / n, z / n);
    }

    Point3 Point3::operator+(const Point3& p) const
    {
        return Point3(x + p.x, y + p.y, z + p.z);
    }

    Point3 Point3::operator-(const Point3& p) const
    {
        return Point3(x - p.x, y - p.y, z - p.z);
    }
}


std::ostream& operator<<(std::ostream& os, const primitives::Point3& point)
{
    return os << "(" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
}