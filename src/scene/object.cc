#include "object.hh"
#include "bump_mapping_texture.hh"

#include <typeinfo>


namespace scene
{
    Object::Object(const TextureMaterial& texture_material)
        : texture_material_(texture_material)
    {}

    primitives::Vector3 Object::get_computed_normal(const primitives::Point3& A) const
    {
        primitives::Vector3 normal = get_normal(A);

        if (typeid(texture_material_) == typeid(BumpMappingTexture))
        {
            const BumpMappingTexture& texture = dynamic_cast<const BumpMappingTexture&>(texture_material_);

            primitives::Point3 planar_projection = get_planar_projection(A);
            normal = normal + texture.get_normal_perturbation(planar_projection);
        }

        return normal.normalize();
    }
}