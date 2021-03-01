#include "engine.hh"

#include <limits>
#include <cmath>
#include <algorithm>

#define RECURSION_LIMIT 2


namespace engine
{
    Engine::Engine(const scene::Scene& scene)
        : scene_(scene)
    {}

    utils::Image Engine::run(uint height, uint width)
    {
        primitives::Vector3* pixels_vector = scene_.camera.get_pixels_vector(height, width);
        primitives::Point3 origin = scene_.camera.get_origin();

        utils::Image res(height, width);

        for (uint j = 0; j < height; ++j)
        {
            for (uint i = 0; i < width; ++i)
            {
                std::optional<primitives::Vector3> intensity = ray_cast(origin, pixels_vector[i + j * width]);

                primitives::Color pixel_color(102, 178, 255);
                if (intensity.has_value())
                {
                    pixel_color = primitives::Color(std::clamp(intensity->dst.x, 0., 255.),
                                std::clamp(intensity->dst.y, 0., 255.),
                                std::clamp(intensity->dst.z, 0., 255.));
                }

                res.pixel(i, j) = pixel_color;
            }
        }

        return res;
    }

    std::optional<primitives::Vector3> Engine::ray_cast(const primitives::Point3& A, const primitives::Vector3& v, uint depth)
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

        primitives::Point3 hitpoint = A + (v * min_lambda).dst;
        const scene::TextureMaterialCaracteristics& hitpoint_desc = closest_object->get_texture(hitpoint);

        primitives::Vector3 res;

        for (const scene::Light* light : scene_.light_sources)
        {
            primitives::Vector3 normal = closest_object->get_normal(hitpoint);

            primitives::Vector3 light_ray(light->get_center() - hitpoint);
            light_ray = light_ray.normalize();

            primitives::Vector3 reflected_ray = (v - normal * v.dot(normal) * 2).normalize();

            primitives::Point3 offset_hitpoint = hitpoint + (normal * 0.0001).dst;

            // Add reflexion
            std::optional<primitives::Vector3> reflexion_contribution = ray_cast(offset_hitpoint, reflected_ray, depth + 1);
            if (reflexion_contribution.has_value())
                res = res + reflexion_contribution.value() * hitpoint_desc.reflection;

            // Take shadow into account
            std::optional<primitives::Vector3> light_check = ray_cast(offset_hitpoint, light_ray, RECURSION_LIMIT);
            if (light_check.has_value())
                continue;

            primitives::Color light_color = light->get_caracteristics().color;
            primitives::Vector3 Li(primitives::Point3(light_color.r, light_color.g, light_color.b));

            primitives::Vector3 Oi(primitives::Point3(hitpoint_desc.color.r, hitpoint_desc.color.g, hitpoint_desc.color.b));

            // Add diffuse component
            res =  res + Oi * Li * hitpoint_desc.kd
                    * std::max(normal.dot(light_ray), 0.);

            // Add specular component
            res = res + Li * hitpoint_desc.ks
                    * pow(std::max(reflected_ray.dot(light_ray), 0.), hitpoint_desc.ns);
        }

        return res;
    }
}