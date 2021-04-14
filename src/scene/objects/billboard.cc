#include "billboard.hh"

#include <cmath>

#define PLANE_EPSILON 1e-6

namespace scene
{
Billboard::Billboard(const std::shared_ptr<TextureMaterial>& texture_material,
                     const Scene&                            scene,
                     const primitives::Point3&               pos,
                     const double                            width,
                     const double                            height)
    : Object(texture_material)
    , pos_(pos)
    , normal_(scene.camera.get_target() * -1.0)
    , width_(width)
    , height_(height)
{
}

std::optional<double>
Billboard::ray_intersection(const primitives::Point3&  A,
                            const primitives::Vector3& v) const
{
    const double normal_dot_v = normal_.dot(v);

    if (std::fabs(normal_dot_v) > PLANE_EPSILON)
    {
        const primitives::Vector3 pos_A = pos_ - A;

        double t = pos_A.dot(normal_) / normal_dot_v;

        if (t >= 0)
        {
            const primitives::Point3  hitpoint = (A + v * t).get_destination();
            const primitives::Vector3 dist     = hitpoint - pos_;

            // CHeck if inside of rectangle and if current pixel is transparent
            if (std::fabs(dist.x) <= width_ && std::fabs(dist.y) <= height_ &&
                get_texture_info(hitpoint).color != primitives::Color(0, 0, 0))
            {
                return t;
            }
        }
    }

    return std::nullopt;
}

primitives::Vector3 Billboard::get_normal(const primitives::Point3&,
                                          const primitives::Vector3&) const
{
    return normal_;
}

primitives::Point3
Billboard::get_planar_projection(const primitives::Point3& A) const
{
    const double a = (pos_.x - A.x) / width_;
    const double b = (pos_.y - A.y) / height_;

    const double u = (a + 1) / 2.0;
    const double v = (b + 1) / 2.0;

    return primitives::Point3(u, 1 - v, 0);
}

TextureMaterialCaracteristics
Billboard::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene