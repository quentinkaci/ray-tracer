#include "color.hh"

namespace primitives
{
    Color::Color(uchar r, uchar g, uchar b)
        : r(r)
        , g(g)
        , b(b)
    {}
}


std::ostream& operator<<(std::ostream& os, const primitives::Color& color)
{
    os << "R: " << color.r << ", G: " << color.g << ", B: " << color.b << std::endl;
    return os;
}