#include <cmath>
#include <random>

#include "transparent_texture.hh"

namespace scene
{
TransparentTexture::TransparentTexture(
    const double                         refractive_index,
    const TextureMaterialCaracteristics& caracteristic)
    : refractive_index_(refractive_index)
    , caracteristic_(caracteristic)
{
}

TextureMaterialCaracteristics
TransparentTexture::get_caracteristics(const primitives::Point3&) const
{
    return caracteristic_;
}

bool TransparentTexture::schlick_approximation(const double n_1,
                                               const double n_2,
                                               const double cos_theta_1) const
{
    // Schlick's approximation
    // (https://en.wikipedia.org/wiki/Schlick%27s_approximation)

    const double r_0     = std::pow((n_1 - n_2) / (n_1 + n_2), 2);
    const double r_theta = r_0 + (1.0 - r_0) * std::pow(1.0 - cos_theta_1, 5);

    // Find a random number between 0 and 1
    std::random_device               rd;
    std::default_random_engine       re(rd());
    std::uniform_real_distribution<> random_value(0, 1);

    // If reflected prob > random number => use reflection over refraction
    return r_theta > random_value(re);
}

primitives::Vector3
TransparentTexture::get_scattered_ray(const primitives::Vector3& ray,
                                      const primitives::Vector3& normal,
                                      SCATTERED_RAY& scattered_ray_type) const
{
    const primitives::Vector3 normal_ =
        (ray.dot(normal) > 0) ? normal * -1.0 : normal;

    // Determine ratio of refractive indexes (1.0 for air)
    const double n1         = (ray.dot(normal) <= 0) ? 1.0 : refractive_index_;
    const double n2         = (ray.dot(normal) <= 0) ? refractive_index_ : 1.0;
    const double n1_over_n2 = n1 / n2;

    // Compute angle between ray and normal
    const double cos_theta_1 =
        std::fmin((ray.normalize() * -1.0).dot(normal_), 1.0);
    const double sin_theta_1 = std::sqrt(1.0 - cos_theta_1 * cos_theta_1);

    // Determine if scattered ray is reflected or refracted
    const bool refraction_is_possible = n1_over_n2 * sin_theta_1 <= 1.0;

    // FIXME: The following condition does not produce expected results.
    // const bool reflect = refraction_is_possible
    //                          ? schlick_approximation(n1, n2, cos_theta_1)
    //                          : true;
    const bool reflect = !refraction_is_possible;

    // The scattered ray is the reflected ray.
    if (reflect)
    {
        const primitives::Vector3 res =
            (ray - normal_ * ray.dot(normal_) * 2).normalize();

        scattered_ray_type = SCATTERED_RAY::REFLECTED;
        return res;
    }

    // The scattered ray is the refracted ray.
    const primitives::Vector3 a =
        (ray.normalize() + normal_ * cos_theta_1) * n1_over_n2;
    const primitives::Vector3 b =
        normal_ * std::sqrt(std::fabs(1.0 - a.norm_squared())) * -1.0;
    const primitives::Vector3 res = a + b;

    scattered_ray_type = SCATTERED_RAY::REFRACTED;
    return res;
}
} // namespace scene