#include "vector.hh"

namespace primitives
{
    Vector3::Vector3(Point3 src, Point3 dst)
        : src(src)
        , dst(dst)
    {}

    Vector3 Vector3::operator*(const float& n) const
    {
        return Vector3(src * n, dst * n);
    }

    Vector3 Vector3::operator-(const Vector3& v) const
    {
        return Vector3(src - v.src, dst - v.dst);
    }
}


std::ostream& operator<<(std::ostream& os, const primitives::Vector3& vector)
{
    os << "[" << vector.src << "; " << vector.dst << "]" << std::endl;
    return os;
}