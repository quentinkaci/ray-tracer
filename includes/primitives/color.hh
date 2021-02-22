#pragma once

#include "types.hh"

#include <iostream>


namespace primitives
{
    class Color
    {
    public:
        Color(uchar r, uchar g, uchar b);

        friend std::ostream& operator<<(std::ostream& os, const Color& color);

    private:
        uchar r_;
        uchar g_;
        uchar b_;
    };
}


std::ostream& operator<<(std::ostream& os, const primitives::Color& color);