#include "perlin_texture.hh"
#include "primitives/vector.hh"

#include <cmath>

namespace scene
{
PerlinTexture::PerlinTexture(uint                                 octave,
                             double                               persistence,
                             const TextureMaterialCaracteristics& caracteristic,
                             PerlinTextureType                    type,
                             std::vector<primitives::Color>       color_list)
    : octave_(octave)
    , persistence_(persistence)
    , caracteristic_(caracteristic)
    , type_(type)
    , color_list_(color_list)
{
}

TextureMaterialCaracteristics
PerlinTexture::get_caracteristics(const primitives::Point3& point) const
{
    TextureMaterialCaracteristics caracteristic(caracteristic_);

    // Retrieve a noise value between 0 and 1
    double noise = perlin_noise_.get_octave(
        point.x, point.y, point.z, octave_, persistence_);

    switch (type_)
    {
    case MARBLE:
        noise = cos(point.x + 2 * noise);
        break;
    case WOOD:
        noise = 20 * noise - floor(20 * noise);
        break;
    default:
        break;
    }

    // Position interval in the list
    noise *= (color_list_.size() - 1);

    int low_bound = floor(noise);

    primitives::Vector3 lc(color_list_[low_bound].r,
                           color_list_[low_bound].g,
                           color_list_[low_bound].b);
    primitives::Vector3 hc(color_list_[low_bound + 1].r,
                           color_list_[low_bound + 1].g,
                           color_list_[low_bound + 1].b);

    primitives::Vector3 interpolation = lc + (hc - lc) * (noise - low_bound);

    caracteristic.color =
        primitives::Color(interpolation.x, interpolation.y, interpolation.z);
    return caracteristic;
}
} // namespace scene