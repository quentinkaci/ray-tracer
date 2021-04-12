#include "skybox.hh"

#include <cmath>

namespace scene
{
Skybox::Skybox(const std::shared_ptr<TextureMaterial>& texture_material,
               const Scene&                            scene)
    : Object(texture_material)
    , sphere_(texture_material, scene.camera.get_origin(), SKYBOX_RADIUS)
{
}

std::optional<double>
Skybox::ray_intersection(const primitives::Point3&  A,
                         const primitives::Vector3& v) const
{
    return sphere_.ray_intersection(A, v);
}

primitives::Vector3 Skybox::get_normal(const primitives::Point3&  A,
                                       const primitives::Vector3& v) const
{
    return sphere_.get_normal(A, v) * -1.0;
}

primitives::Point3
Skybox::get_planar_projection(const primitives::Point3& A) const
{
    return sphere_.get_planar_projection(A);
}

TextureMaterialCaracteristics
Skybox::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene