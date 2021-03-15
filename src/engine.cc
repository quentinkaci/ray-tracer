#include "engine.hh"

#include <limits>
#include <cmath>
#include <algorithm>
#include <random>

#define RECURSION_LIMIT 5
#define EPSILON 0.0001
#define NB_RAY_PER_PIXEL 1
#define BACKGROUND_COLOR primitives::Vector3(102., 178., 255.)

namespace engine
{
    Engine::Engine(const scene::Scene& scene)
        : scene_(scene)
    {}

    utils::Image Engine::run(uint height, uint width)
    {
        primitives::Vector3* pixels_vector = scene_.camera.get_pixels_vector(height, width);
        primitives::Point3 origin = scene_.camera.get_origin();

        double unit_x = scene_.camera.get_unit_x(width);
        double unit_y = scene_.camera.get_unit_y(height);

        std::uniform_real_distribution<double> unif_x(- unit_x / 2., unit_x / 2.);
        std::uniform_real_distribution<double> unif_y(- unit_y / 2., unit_y / 2.);
        std::default_random_engine re;

        utils::Image res(height, width);

        for (uint j = 0; j < height; ++j)
        {
            for (uint i = 0; i < width; ++i)
            {
                primitives::Vector3 average_intensity;

                for (uint k = 0; k < NB_RAY_PER_PIXEL; ++k)
                {
                    primitives::Vector3 pixel_vector = pixels_vector[i + j * width];
                    pixel_vector.x += unif_x(re);
                    pixel_vector.y += unif_y(re);

                    std::optional<primitives::Vector3> intensity = cast_ray(origin, pixel_vector);
                    if (!intensity.has_value())
                        intensity = BACKGROUND_COLOR;

                    average_intensity = average_intensity + intensity.value();
                }

                average_intensity = average_intensity / (double) NB_RAY_PER_PIXEL;

                primitives::Color pixel_color = primitives::Color(
                                                std::clamp(average_intensity.x, 0., 255.),
                                                std::clamp(average_intensity.y, 0., 255.),
                                                std::clamp(average_intensity.z, 0., 255.));

                res.pixel(i, j) = pixel_color;
            }
        }

        return res;
    }

    std::optional<primitives::Vector3> Engine::cast_ray(const primitives::Point3& A, const primitives::Vector3& v, uint depth)
    {
        double min_lambda = std::numeric_limits<double>::infinity();
        const scene::Object* closest_object = nullptr;

        for (const scene::Object* object : scene_.objects)
        {
            std::optional<double> lambda = object->ray_intersection(A, v);
            if (lambda.has_value() && lambda.value() < min_lambda)
            {
                min_lambda = lambda.value();
                closest_object = object;
            }
        }

        // No object in ray direction
        if (closest_object == nullptr)
            return std::nullopt;

        if (depth >= RECURSION_LIMIT)
            return primitives::Vector3();

        primitives::Point3 hitpoint = A + (v * min_lambda).get_destination();
        const scene::TextureMaterialCaracteristics& hitpoint_desc = closest_object->get_texture(hitpoint);
        primitives::Vector3 object_color(hitpoint_desc.color.r, hitpoint_desc.color.g, hitpoint_desc.color.b);

        primitives::Vector3 normal = closest_object->get_normal(hitpoint);
        primitives::Vector3 reflected_ray = (v - normal * v.dot(normal) * 2).normalize();
        primitives::Point3 offset_hitpoint = hitpoint + (normal * EPSILON).get_destination();

        primitives::Vector3 res;

        for (const scene::Light* light : scene_.light_sources)
        {
            primitives::Vector3 light_ray(light->get_center() - hitpoint);
            light_ray = light_ray.normalize();

            // Take shadow into account
            std::optional<primitives::Vector3> light_check = cast_ray(offset_hitpoint, light_ray, RECURSION_LIMIT);
            // Obstacle betweem hitpoint and light
            if (light_check.has_value())
                continue;

            primitives::Color lc = light->get_caracteristics().color;
            primitives::Vector3 light_color(lc.r, lc.g, lc.b);

            // Add diffuse component
            res = res + object_color * light_color * hitpoint_desc.kd
                    * std::max(normal.dot(light_ray), 0.);

            // Add specular component
            res = res + light_color * hitpoint_desc.ks
                    * pow(std::max(reflected_ray.dot(light_ray), 0.), hitpoint_desc.ns);
        }

        // Add reflexion
        std::optional<primitives::Vector3> reflection_contribution = cast_ray(offset_hitpoint, reflected_ray, depth + 1);
        if (reflection_contribution.has_value())
            res = res + reflection_contribution.value() * hitpoint_desc.reflection;

        return res;
    }
}