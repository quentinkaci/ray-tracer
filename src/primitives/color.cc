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

    std::ostream& operator<<(std::ostream& os, const primitives::Color& color)
    {
        return os << "R: " << +color.r << ", G: " << +color.g << ", B: " << +color.b << std::endl;
    }
}