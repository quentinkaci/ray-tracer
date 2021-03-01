#pragma once

#include "types.hh"

#include <iostream>


namespace primitives
{
    struct Color
    {
        Color(uchar r, uchar g, uchar b);

        Color();

        Color operator*(const Color& color) const;

        Color operator+(const Color& color) const;

        Color operator*(const double& n) const;

        uchar r;
        uchar g;
        uchar b;
    };
}


std::ostream& operator<<(std::ostream& os, const primitives::Color& color);