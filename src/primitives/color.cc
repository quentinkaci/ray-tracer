#include "color.hh"

namespace primitives
{
Color::Color(uchar r, uchar g, uchar b)
    : r(r)
    , g(g)
    , b(b)
{
}

Color::Color()
    : Color(0, 0, 0)
{
}

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

bool operator!=(const Color& lhs, const Color& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, const primitives::Color& color)
{
    return os << "R: " << +color.r << ", G: " << +color.g << ", B: " << +color.b
              << std::endl;
}
} // namespace primitives