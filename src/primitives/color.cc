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
uchar Color::to_grayscale() const
{
    return (uchar)(0.2989 * (double)r + 0.5870 * (double)g +
                   0.1140 * (double)b);
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