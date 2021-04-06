#include "engine.hh"

#include <algorithm>
#include <cmath>
#include <limits>

#define EPSILON 0.0001

// Reflection
#define RECURSION_LIMIT 5

// Anti-aliasing
#define NB_RAY_AA 20
#define AA_THRESHOLD 10

// Ambient color
#define BACKGROUND_COLOR primitives::Vector3(102., 178., 255.)

// Soft shadow
#define NB_RAY_SOFT_SHADOW 20
#define MIN_RANGE_SOFT_SHADOW -1
#define MAX_RANGE_SOFT_SHADOW 1

// Depth of field
#define NB_RAY_DOF 20
#define FOCAL_DISTANCE 10
#define APERTURE_SIZE 250

namespace engine
{
Engine::Engine(const scene::Scene& scene)
    : scene_(scene)
{
    std::random_device rd;
    unsigned           seed = rd();
    re_                     = std::default_random_engine(seed);
}

static double gradient(const primitives::Color&   color,
                       const primitives::Vector3& vector)
{
    return sqrt(pow(color.r - vector.x, 2) + pow(color.g - vector.y, 2) +
                pow(color.b - vector.z, 2));
}

utils::Image Engine::run(uint height, uint width)
{
    primitives::Vector3* pixels_vector =
        scene_.camera.get_pixels_vector(height, width);
    primitives::Point3 origin = scene_.camera.get_origin();

    double unit_x = scene_.camera.get_unit_x(width);
    double unit_y = scene_.camera.get_unit_y(height);

    std::uniform_real_distribution<double> aa_unif_x(-unit_x / 2., unit_x / 2.);
    std::uniform_real_distribution<double> aa_unif_y(-unit_y / 2., unit_y / 2.);

    std::uniform_real_distribution<double> dof_unif_x(-unit_x / 2.,
                                                      unit_x / 2.);
    std::uniform_real_distribution<double> dof_unif_y(-unit_y / 2.,
                                                      unit_y / 2.);

    utils::Image res(height, width);

    for (uint j = 0; j < height; ++j)
    {
        for (uint i = 0; i < width; ++i)
        {
            std::vector<primitives::Color> neighbours;

            if (j > 0)
            {
                if (i > 0)
                    neighbours.emplace_back(res.get_pixel(i - 1, j - 1));
                if (i < width - 1)
                    neighbours.emplace_back(res.get_pixel(i + 1, j - 1));

                neighbours.emplace_back(res.get_pixel(i, j - 1));
            }

            if (i > 0)
                neighbours.emplace_back(res.get_pixel(i - 1, j));

            primitives::Vector3 aa_intensity;

            uint k = 0;
            while (k < NB_RAY_AA)
            {
                primitives::Vector3 random_vector =
                    pixels_vector[i + j * width];
                random_vector.x += aa_unif_x(re_);
                random_vector.y += aa_unif_y(re_);
                random_vector = random_vector.normalize();

                primitives::Vector3 dof_intensity;
                for (uint l = 0; l < NB_RAY_DOF; ++l)
                {
                    primitives::Vector3 focal_point =
                        origin + random_vector * FOCAL_DISTANCE;

                    primitives::Point3 jittered_origin(origin);
                    jittered_origin.x += APERTURE_SIZE * dof_unif_x(re_);
                    jittered_origin.y += APERTURE_SIZE * dof_unif_y(re_);

                    primitives::Vector3 pixel_vector =
                        focal_point - jittered_origin;

                    std::optional<primitives::Vector3> intensity =
                        cast_ray(jittered_origin, pixel_vector);
                    if (!intensity.has_value())
                        intensity = BACKGROUND_COLOR;

                    dof_intensity = dof_intensity + intensity.value();
                }

                dof_intensity = dof_intensity / NB_RAY_DOF;

                aa_intensity = aa_intensity + dof_intensity;

                ++k;

                bool over_threshold = false;
                for (const auto& neighbour : neighbours)
                    over_threshold |=
                        gradient(neighbour, aa_intensity) > AA_THRESHOLD;

                if (!over_threshold)
                    break;
            }

            aa_intensity = aa_intensity / k;

            primitives::Color pixel_color =
                primitives::Color(std::clamp(aa_intensity.x, 0., 255.),
                                  std::clamp(aa_intensity.y, 0., 255.),
                                  std::clamp(aa_intensity.z, 0., 255.));

            res.pixel(i, j) = pixel_color;
        }
    }

    return res;
}

std::optional<primitives::Vector3>
Engine::cast_ray(const primitives::Point3&  A,
                 const primitives::Vector3& v,
                 uint                       depth)
{
    double               min_lambda = std::numeric_limits<double>::infinity();
    const scene::Object* closest_object = nullptr;

    for (const scene::Object* object : scene_.objects)
    {
        std::optional<double> lambda = object->ray_intersection(A, v);
        if (lambda.has_value() && lambda.value() < min_lambda)
        {
            min_lambda     = lambda.value();
            closest_object = object;
        }
    }

    // No object in ray direction or same object that reflect in himself
    if (closest_object == nullptr ||
        (depth == 1 && last_reflected_object_ == closest_object))
        return std::nullopt;

    if (depth >= RECURSION_LIMIT)
        return primitives::Vector3();

    primitives::Point3 hitpoint = A + (v * min_lambda).get_destination();
    const scene::TextureMaterialCaracteristics& hitpoint_desc =
        closest_object->get_texture(hitpoint);
    primitives::Vector3 object_color(
        hitpoint_desc.color.r, hitpoint_desc.color.g, hitpoint_desc.color.b);

    primitives::Vector3 normal =
        closest_object->get_computed_normal(hitpoint, v);
    primitives::Vector3 reflected_ray =
        (v - normal * v.dot(normal) * 2).normalize();
    primitives::Point3 offset_hitpoint =
        hitpoint + (normal * EPSILON).get_destination();

    primitives::Vector3 res;

    std::uniform_real_distribution<double> unif_x(MIN_RANGE_SOFT_SHADOW,
                                                  MAX_RANGE_SOFT_SHADOW);
    std::uniform_real_distribution<double> unif_y(MIN_RANGE_SOFT_SHADOW,
                                                  MAX_RANGE_SOFT_SHADOW);
    std::uniform_real_distribution<double> unif_z(MIN_RANGE_SOFT_SHADOW,
                                                  MAX_RANGE_SOFT_SHADOW);

    for (const scene::Light* light : scene_.light_sources)
    {
        primitives::Vector3 light_ray(light->get_center() - hitpoint);
        light_ray = light_ray.normalize();

        double shadow_coef = 0.;

        for (uint k = 0; k < NB_RAY_SOFT_SHADOW; ++k)
        {
            primitives::Point3 jittered_light =
                light->get_center() +
                primitives::Point3(unif_x(re_), unif_y(re_), unif_z(re_));

            primitives::Vector3 random_light_ray(jittered_light - hitpoint);
            random_light_ray = random_light_ray.normalize();

            // Take shadow into account
            std::optional<primitives::Vector3> light_check =
                cast_ray(offset_hitpoint, random_light_ray, RECURSION_LIMIT);
            // Obstacle between hitpoint and light
            if (light_check.has_value())
                ++shadow_coef;
        }

        primitives::Color   lc = light->get_caracteristics().color;
        primitives::Vector3 light_color(lc.r, lc.g, lc.b);

        // Add diffuse component
        res = res + object_color * light_color * hitpoint_desc.kd *
                        std::max(normal.dot(light_ray), 0.);

        // Add specular component
        res = res + light_color * hitpoint_desc.ks *
                        pow(std::max(reflected_ray.dot(light_ray), 0.),
                            hitpoint_desc.ns);

        // Apply shadow
        res = res * (1 - (shadow_coef / NB_RAY_SOFT_SHADOW));
    }

    last_reflected_object_ = closest_object;

    // Add reflexion
    std::optional<primitives::Vector3> reflection_contribution =
        cast_ray(offset_hitpoint, reflected_ray, depth + 1);
    if (reflection_contribution.has_value())
        res = res + reflection_contribution.value() * hitpoint_desc.reflection;

    return res;
}
} // namespace engine