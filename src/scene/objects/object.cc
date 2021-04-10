#include "object.hh"
#include "scene/textures/bump_mapping_texture.hh"

#include <typeinfo>
#include <utility>

namespace scene
{
Object::Object(std::shared_ptr<TextureMaterial> texture_material)
    : texture_material_(std::move(texture_material))
{
}

const std::shared_ptr<TextureMaterial>& Object::get_texture() const { return texture_material_; }

primitives::Vector3
Object::get_computed_normal(const primitives::Point3&  A,
                            const primitives::Vector3& v) const
{
    primitives::Vector3 normal = get_normal(A, v);

    if (typeid(*texture_material_) == typeid(BumpMappingTexture))
    {
        const auto& texture =
            dynamic_cast<const BumpMappingTexture&>(*texture_material_);

        primitives::Point3  planar_projection = get_planar_projection(A);
        primitives::Vector3 pertubation =
            texture.get_normal_perturbation(planar_projection);

        primitives::Vector3 u =
            primitives::Vector3(normal.x + 1, normal.y, normal.z) -
            normal * (normal.x + 1);
        // Collinear case
        if ((normal.x == 1 || normal.x == -1) && normal.y == 0 && normal.z == 0)
            u = primitives::Vector3(0, 1, 0);

        u = u.normalize();

        primitives::Vector3 v = u.cross(normal).normalize();

        normal = (normal + u * pertubation.x + v * pertubation.y).normalize();
    }

    return normal;
}
} // namespace scene