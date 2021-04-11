#include <cmath>

#include "procedural_texture.hh"

namespace scene
{
ProceduralTexture::ProceduralTexture(
    const std::string&                   style,
    const double                         size,
    const TextureMaterialCaracteristics& caracteristic)
    : style_(style)
    , size_(size)
    , caracteristic_(caracteristic)
{
}

inline float modulo(const float& x) { return x - std::floor(x); }

TextureMaterialCaracteristics
ProceduralTexture::get_caracteristics(const primitives::Point3& A) const
{
    TextureMaterialCaracteristics res = caracteristic_;

    if (style_ == "checkerboard")
    {
        double color =
            (modulo(A.y * 1 / size_) < 0.5) ^ (modulo(A.x * 1 / size_) < 0.5);
        res.color = primitives::Color(
            color * res.color.r, color * res.color.g, color * res.color.b);
    }
    else if (style_ == "stripes")
    {
        double color = (std::sin(A.x * 2 * M_PI * 1 / size_) + 1) * 0.5;
        res.color    = primitives::Color(
            color * res.color.r, color * res.color.g, color * res.color.b);
    }
    else // style_ == "gradient"
    {
        double tx = (A.x + 1) * 0.5;
        double ty = (A.y + 1) * 0.5;
        double t  = (tx + ty) / 2;
        res.color = primitives::Color(
            t * res.color.r, t * res.color.g, t * res.color.b);
    }

    return res;
}
} // namespace scene