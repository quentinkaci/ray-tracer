#include "engine.hh"
#include "scene/transparent_texture.hh"

#include <algorithm>
#include <cmath>
#include <limits>
#include <typeinfo>

#define EPSILON 0.0001

// Reflection / refraction
#define RECURSION_LIMIT 3

// Anti-aliasing
#define MAX_RAY_PER_PIXEL 1
#define AA_THRESHOLD -1

// Ambient color
#define BACKGROUND_COLOR primitives::Vector3(102., 178., 255.)

// Soft shadow
#define NB_RAY_SOFT_SHADOW 1

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

    std::uniform_real_distribution<double> unif_x(-unit_x / 2., unit_x / 2.);
    std::uniform_real_distribution<double> unif_y(-unit_y / 2., unit_y / 2.);

    utils::Image res(height, width);

    for (uint j = 0; j < height; ++j)
    {
        for (uint i = 0; i < width; ++i)
        {
            const primitives::Vector3& pixel_vector =
                pixels_vector[i + j * width];

            std::optional<primitives::Vector3> primary_intensity =
                cast_ray(origin, pixel_vector);
            if (!primary_intensity.has_value())
                primary_intensity = BACKGROUND_COLOR;

            primitives::Vector3 interpolated_intensity =
                primary_intensity.value();

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

            uint k = 1;
            for (; k < MAX_RAY_PER_PIXEL; ++k)
            {
                bool over_threshold = false;
                for (const auto& neighbour : neighbours)
                    over_threshold |=
                        gradient(neighbour, interpolated_intensity) >
                        AA_THRESHOLD;

                if (!over_threshold)
                    break;

                primitives::Vector3 random_vector =
                    pixels_vector[i + j * width];
                random_vector.x += unif_x(re_);
                random_vector.y += unif_y(re_);
                random_vector.normalize();

                std::optional<primitives::Vector3> new_intensity =
                    cast_ray(origin, random_vector);
                if (!new_intensity.has_value())
                    new_intensity = BACKGROUND_COLOR;

                interpolated_intensity =
                    (interpolated_intensity * ((double)MAX_RAY_PER_PIXEL - 1.) +
                     new_intensity.value()) /
                    (double)MAX_RAY_PER_PIXEL;
            }

            primitives::Color pixel_color = primitives::Color(
                std::clamp(interpolated_intensity.x, 0., 255.),
                std::clamp(interpolated_intensity.y, 0., 255.),
                std::clamp(interpolated_intensity.z, 0., 255.));

            res.pixel(i, j) = pixel_color;
        }
    }

    return res;
}

bool Engine::cast_ray_light_check(const primitives::Point3&  A,
                                  const primitives::Vector3& v)
{
    double               min_lambda = std::numeric_limits<double>::infinity();
    const scene::Object* closest_object = nullptr;

    for (const scene::Object* object : scene_.objects)
    {
        std::optional<double> lambda = object->ray_intersection(A, v);
        if (lambda.has_value() && lambda.value() < min_lambda &&
            typeid(object->get_texture()) != typeid(scene::TransparentTexture))
        {
            min_lambda     = lambda.value();
            closest_object = object;
        }
    }

    return closest_object != nullptr;
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
        (depth == 2 && last_reflected_object_ == closest_object))
    {
        auto t = 0.5 * (v.normalize().y + 1.0);
        return primitives::Vector3(255, 255, 255) * (1.0 - t) +
               primitives::Vector3(128, 178, 255) * t;
    }

    if (depth >= RECURSION_LIMIT)
        return primitives::Vector3(0, 0, 0);

    primitives::Point3 hitpoint = A + (v * min_lambda).get_destination();
    const scene::TextureMaterialCaracteristics& hitpoint_desc =
        closest_object->get_texture_info(hitpoint);
    primitives::Vector3 object_color(
        hitpoint_desc.color.r, hitpoint_desc.color.g, hitpoint_desc.color.b);

    primitives::Vector3 normal =
        closest_object->get_computed_normal(hitpoint, v);
    primitives::Vector3 reflected_ray =
        (v - normal * v.dot(normal) * 2).normalize();
    primitives::Point3 offset_hitpoint =
        hitpoint + (normal * EPSILON).get_destination();

    primitives::Vector3 res;

    std::uniform_real_distribution<double> unif_x(-0.5, 0.5);
    std::uniform_real_distribution<double> unif_y(-0.5, 0.5);
    std::uniform_real_distribution<double> unif_z(-0.5, 0.5);

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

            auto light_check =
                cast_ray_light_check(offset_hitpoint, random_light_ray);

            // Obstacle betweem hitpoint and light
            if (light_check)
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
        res = res - primitives::Vector3(150, 150, 150) *
                        (shadow_coef / NB_RAY_SOFT_SHADOW);
        res = primitives::Vector3(
            std::max(res.x, 0.), std::max(res.y, 0.), std::max(res.z, 0.));
    }

    last_reflected_object_ = closest_object;

    // Add reflexion / refraction
    primitives::Vector3 scattered_ray =
        closest_object->get_texture().get_scattered_ray(v, normal);

    if (typeid(closest_object->get_texture()) ==
        typeid(scene::TransparentTexture))
    {
        offset_hitpoint =
            hitpoint + (v.normalize() * EPSILON).get_destination();
    }

    std::optional<primitives::Vector3> reflection_contribution =
        cast_ray(offset_hitpoint, scattered_ray, depth + 1);

    if (reflection_contribution.has_value())
        res = res + reflection_contribution.value() * hitpoint_desc.reflection;

    return res;
}
} // namespace engine