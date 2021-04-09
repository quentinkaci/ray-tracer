#include "vector.hh"

#include <cmath>

namespace primitives
{
Vector3::Vector3(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vector3::Vector3(const Point3& point)
    : x(point.x)
    , y(point.y)
    , z(point.z)
{
}

Vector3::Vector3()
    : Vector3(0, 0, 0)
{
}

double Vector3::dot(const Vector3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Point3 Vector3::get_destination() const { return Point3(x, y, z); }

double Vector3::norm() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

double Vector3::norm_squared() const
{
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

Vector3 Vector3::normalize() const
{
    double length = norm();

    return Vector3(x / length, y / length, z / length);
}

bool operator==(const Vector3& lhs, const Vector3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
    return !(lhs == rhs);
}

Vector3 operator*(const Vector3& v, const double& n)
{
    return Vector3(v.x * n, v.y * n, v.z * n);
}

Vector3 operator/(const Vector3& v, const double& n)
{
    return Vector3(v.x / n, v.y / n, v.z / n);
}

Vector3 operator*(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vector3 operator/(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
}

Vector3 operator+(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3 operator-(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    return os << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}
} // namespace primitives