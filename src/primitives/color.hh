#pragma once

#include "utils/types.hh"

#include <iostream>

namespace primitives
{
struct Color
{
    Color(uchar r, uchar g, uchar b);

    Color();

    uchar to_grayscale() const;

    uchar r;
    uchar g;
    uchar b;
};

bool operator==(const Color& lhs, const Color& rhs);

bool operator!=(const Color& lhs, const Color& rhs);

std::ostream& operator<<(std::ostream& os, const primitives::Color& color);
} // namespace primitives