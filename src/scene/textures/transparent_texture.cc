#include <cmath>

#include "transparent_texture.hh"

namespace scene
{
TransparentTexture::TransparentTexture(
    const TextureMaterialCaracteristics& caracteristic)
    : caracteristic_(caracteristic)
{
}

TextureMaterialCaracteristics
TransparentTexture::get_caracteristics(const primitives::Point3&) const
{
    return caracteristic_;
}

primitives::Vector3
TransparentTexture::get_scattered_ray(const primitives::Vector3& ray,
                                      const primitives::Vector3& normal) const
{
    primitives::Vector3 u       = ray.normalize();
    primitives::Vector3 normal_ = normal;

    double ir = 1.5;

    if (ray.dot(normal) <= 0)
        ir = 1.0 / ir;
    else
        normal_ = normal * -1.0;

    double cos_theta = std::fmin((u * -1.0).dot(normal_), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool can_refract = ir * sin_theta <= 1.0;

    if (can_refract)
    {
        // The scattered ray is the refracted ray.
        primitives::Vector3 r_out_perp = (u + normal_ * cos_theta) * ir;
        primitives::Vector3 r_out_parallel =
            normal_ * std::sqrt(std::fabs(1.0 - r_out_perp.norm_squared())) *
            -1.0;
        return r_out_perp + r_out_parallel;
    }

    // The scattered ray is the reflected ray.
    return (ray - normal_ * ray.dot(normal_) * 2).normalize();
}
} // namespace scene