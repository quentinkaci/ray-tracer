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

TextureMaterialCaracteristics
ProceduralTexture::get_caracteristics(const primitives::Point3& A) const
{
    TextureMaterialCaracteristics res = caracteristic_;

    if (style_ == "checkerboard")
    {
        const uint   tx    = std::fmod(A.x / size_, 1) < 0.5;
        const uint   ty    = std::fmod(A.y / size_, 1) < 0.5;
        const double color = tx ^ ty;

        res.color = primitives::Color(
            color * res.color.r, color * res.color.g, color * res.color.b);
    }
    else if (style_ == "stripes_y")
    {
        const double color = (std::sin(A.x * 2 * M_PI * 1 / size_) + 1) * 0.5;

        res.color = primitives::Color(
            color * res.color.r, color * res.color.g, color * res.color.b);
    }
    else if (style_ == "stripes_x")
    {
        const double color = (std::cos(A.y * 2 * M_PI * 1 / size_) + 1) * 0.5;

        res.color = primitives::Color(
            color * res.color.r, color * res.color.g, color * res.color.b);
    }
    else if (style_ == "gradient_xy")
    {
        const double tx = (A.x + 1) * 0.5;
        const double ty = (A.y + 1) * 0.5;
        const double t  = (tx + ty) / 2;

        res.color = primitives::Color(
            t * res.color.r, t * res.color.g, t * res.color.b);
    }
    else if (style_ == "gradient_x")
    {
        res.color = primitives::Color(
            A.x * res.color.r, A.x * res.color.g, A.x * res.color.b);
    }
    else // (style_ == "gradient_y")
    {
        res.color = primitives::Color(
            A.y * res.color.r, A.y * res.color.g, A.y * res.color.b);
    }

    return res;
}
} // namespace scene