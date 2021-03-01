#include "color.hh"

namespace primitives
{
    Color::Color(uchar r, uchar g, uchar b)
        : r(r)
        , g(g)
        , b(b)
    {}

    Color::Color()
        : Color(0, 0, 0)
    {}

    Color Color::operator*(const Color& color) const
    {
        return Color(r * color.r, g * color.g, b * color.b);
    }

    Color Color::operator+(const Color& color) const
    {
        return Color(r + color.r, g + color.g, b + color.b);
    }

    Color Color::operator*(const double& n) const
    {
        return Color(r * n, g * n, b * n);
    }
}


std::ostream& operator<<(std::ostream& os, const primitives::Color& color)
{
    os << "R: " << +color.r << ", G: " << +color.g << ", B: " << +color.b << std::endl;
    return os;
}