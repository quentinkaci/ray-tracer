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
            primitives::Vector3 pertubation = texture.get_normal_perturbation(planar_projection);

            primitives::Vector3 u = primitives::Vector3(normal.x + 1, normal.y, normal.z) - normal * (normal.x + 1);
            // Collinear case
            if ((normal.x == 1 || normal.x == -1) && normal.y == 0 && normal.z == 0)
                u = primitives::Vector3(0, 1, 0);

            u = u.normalize();

            primitives::Vector3 v = u.cross(normal).normalize();

            normal = (normal + u * pertubation.x + v * pertubation.y).normalize();
        }

        return normal;
    }
}