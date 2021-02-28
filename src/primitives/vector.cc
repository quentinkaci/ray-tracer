#include "vector.hh"

#include <cmath>

namespace primitives
{
    Vector3::Vector3(Point3 dst)
        : dst(dst)
    {}

    Vector3 Vector3::operator*(const double& n) const
    {
        return Vector3(dst * n);
    }

    Vector3 Vector3::operator+(const Vector3& v) const
    {
        return Vector3(dst + v.dst);
    }

    Vector3 Vector3::operator-(const Vector3& v) const
    {
        return Vector3(dst - v.dst);
    }

    double Vector3::dot(const Vector3& v) const
    {
        return dst.x * v.dst.x + dst.y * v.dst.y + dst.z * v.dst.z;
    }

    Vector3 Vector3::cross(const Vector3& v) const
    {
        return Vector3(primitives::Point3(dst.y * v.dst.z - dst.z * v.dst.y
                                        , dst.z * v.dst.x - dst.x * v.dst.z
                                        , dst.x * v.dst.y - dst.y * v.dst.x));
    }

    void Vector3::normalize()
    {
        uint length = sqrt(pow(dst.x, 2) + pow(dst.y, 2) + pow(dst.z, 2));

        dst.x /= length;
        dst.y /= length;
        dst.z /= length;
    }
}


std::ostream& operator<<(std::ostream& os, const primitives::Vector3& vector)
{
    return os << "[" << vector.dst << "]" << std::endl;
}