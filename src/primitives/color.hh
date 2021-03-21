#pragma once

#include "types.hh"

#include <iostream>

namespace primitives
{
struct Color
{
    Color(uchar r, uchar g, uchar b);

    Color();

    uchar r;
    uchar g;
    uchar b;
};

std::ostream& operator<<(std::ostream& os, const primitives::Color& color);
} // namespace primitives