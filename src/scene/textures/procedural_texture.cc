#include <cmath>

#include "procedural_texture.hh"

namespace scene
{
ProceduralTexture::ProceduralTexture(
    const TextureMaterialCaracteristics& caracteristic)
    : caracteristic_(caracteristic)
{
}

inline float modulo(const float& x) { return x - std::floor(x); }

TextureMaterialCaracteristics
ProceduralTexture::get_caracteristics(const primitives::Point3& A) const
{
    TextureMaterialCaracteristics res = caracteristic_;

    float size = 2;
    // float color = (std::sin(A.x * 2 * M_PI * size) + 1) * 0.5;
    float color = (modulo(A.y * size) < 0.5) ^ (modulo(A.x * size) < 0.5);

    // res.color = primitives::Color(A.x * 255, A.y * 255, 0.5 * 255);
    res.color = primitives::Color(color * 255, color * 255, color * 255);
    return res;
}
} // namespace scene