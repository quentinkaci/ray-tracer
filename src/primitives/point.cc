#include "point.hh"

namespace primitives
{
Point3::Point3(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{
}

Point3::Point3()
    : Point3(0, 0, 0)
{
}

Point3 operator*(const Point3& point, const double& n)
{
    return Point3(point.x * n, point.y * n, point.z * n);
}

Point3 operator/(const Point3& point, const double& n)
{
    return Point3(point.x / n, point.y / n, point.z / n);
}

Point3 operator+(const Point3& left, const Point3& right)
{
    return Point3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Point3 operator-(const Point3& left, const Point3& right)
{
    return Point3(left.x - right.x, left.y - right.y, left.z - right.z);
}

std::ostream& operator<<(std::ostream& os, const primitives::Point3& point)
{
    return os << "(" << point.x << ", " << point.y << ", " << point.z << ")"
              << std::endl;
}
} // namespace primitives