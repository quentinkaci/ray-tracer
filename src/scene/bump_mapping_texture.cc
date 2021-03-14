#include "bump_mapping_texture.hh"

#define EPSILON 0.01


namespace scene
{
    BumpMappingTexture::BumpMappingTexture(uint octave, double persistence, const TextureMaterialCaracteristics& caracteristic)
        : octave_(octave)
        , persistence_(persistence)
        , caracteristic_(caracteristic)
    {}

    TextureMaterialCaracteristics BumpMappingTexture::get_caracteristics(const primitives::Point3&) const
    {
        return caracteristic_;
    }

    primitives::Vector3 BumpMappingTexture::get_normal_perturbation(const primitives::Point3& point) const
    {
        double z = 0.5;

        return primitives::Vector3
        (
            p_.get_octave(point.x - EPSILON, point.y, z, octave_, persistence_)
                - p_.get_octave(point.x + EPSILON, point.y, z, octave_, persistence_),
            p_.get_octave(point.x, point.y - EPSILON, z, octave_, persistence_)
                - p_.get_octave(point.x, point.y + EPSILON, z, octave_, persistence_),
            0.
        );
    }
}