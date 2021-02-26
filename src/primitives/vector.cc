#include "vector.hh"

namespace primitives
{
    Vector3::Vector3(Point3 dst)
        : dst(dst)
    {}

    Vector3 Vector3::operator*(const float& n) const
    {
        return Vector3(dst * n);
    }

    Vector3 Vector3::operator-(const Vector3& v) const
    {
        return Vector3(dst - v.dst);
    }
}


std::ostream& operator<<(std::ostream& os, const primitives::Vector3& vector)
{
    return os << "[" << vector.dst << "]" << std::endl;
}