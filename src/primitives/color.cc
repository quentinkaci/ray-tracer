#include "color.hh"

namespace primitives
{
    Color::Color(uchar r, uchar g, uchar b)
        : r_(r)
        , g_(g)
        , b_(b)
    {}
}


std::ostream& operator<<(std::ostream& os, const primitives::Color& color)
{
    os << "R: " << color.r_ << ", G: " << color.g_ << ", B: " << color.b_ << std::endl;
    return os;
}