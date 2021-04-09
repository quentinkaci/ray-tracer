#include "engine.hh"
#include "scene/transparent_texture.hh"

#include <algorithm>
#include <cmath>
#include <limits>
#include <typeinfo>

#define EPSILON 0.0001

namespace core
{
Engine::Engine(const Options& options, const scene::Scene& scene)
    : options_(options)
    , scene_(scene)
{
    std::random_device rd;
    unsigned           seed = rd();
    re_                     = std::default_random_engine(seed);
}

void Engine::init_distributions(uint height, uint width)
{
    double unit_x = scene_.camera.get_unit_x(width);
    double unit_y = scene_.camera.get_unit_y(height);

    aa_unif_x_ =
        std::uniform_real_distribution<double>(-unit_x / 2., unit_x / 2.);
    aa_unif_y_ =
        std::uniform_real_distribution<double>(-unit_y / 2., unit_y / 2.);

    dof_unif_x_ =
        std::uniform_real_distribution<double>(-unit_x / 2., unit_x / 2.);
    dof_unif_y_ =
        std::uniform_real_distribution<double>(-unit_y / 2., unit_y / 2.);

    soft_shadow_unif_x_ = std::uniform_real_distribution<double>(
        options_.min_range_soft_shadow, options_.max_range_soft_shadow);
    soft_shadow_unif_y_ = std::uniform_real_distribution<double>(
        options_.min_range_soft_shadow, options_.max_range_soft_shadow);
    soft_shadow_unif_z_ = std::uniform_real_distribution<double>(
        options_.min_range_soft_shadow, options_.max_range_soft_shadow);
}

primitives::Vector3
Engine::compute_depth_of_field(const primitives::Point3&  origin,
                               const primitives::Vector3& vector)
{
    if (!options_.dof_enabled)
    {
        std::optional<primitives::Vector3> intensity = cast_ray(origin, vector);
        if (!intensity.has_value())
            intensity = options_.ambient_color;

        return intensity.value();
    }

    primitives::Vector3 dof_intensity;
    for (uint l = 0; l < options_.nb_ray_dof; ++l)
    {
        primitives::Vector3 focal_point =
            origin + vector * options_.focal_distance;

        primitives::Point3 jittered_origin(origin);
        jittered_origin.x += options_.aperture_size * dof_unif_x_(re_);
        jittered_origin.y += options_.aperture_size * dof_unif_y_(re_);

        primitives::Vector3 pixel_vector = focal_point - jittered_origin;

        std::optional<primitives::Vector3> intensity =
            cast_ray(jittered_origin, pixel_vector);
        if (!intensity.has_value())
            intensity = options_.ambient_color;

        dof_intensity = dof_intensity + intensity.value();
    }

    return dof_intensity / options_.nb_ray_dof;
}

static double gradient(const primitives::Color&   color,
                       const primitives::Vector3& vector)
{
    return sqrt(pow(color.r - vector.x, 2) + pow(color.g - vector.y, 2) +
                pow(color.b - vector.z, 2));
}

primitives::Vector3
Engine::compute_anti_aliasing(const primitives::Point3&             origin,
                              const primitives::Vector3&            vector,
                              const std::vector<primitives::Color>& neighbours)
{
    if (!options_.aa_enabled)
        return compute_depth_of_field(origin, vector);

    primitives::Vector3 aa_intensity;

    uint k = 0;
    while (k < options_.nb_ray_aa)
    {
        primitives::Vector3 random_vector = vector;
        random_vector.x += aa_unif_x_(re_);
        random_vector.y += aa_unif_y_(re_);
        random_vector = random_vector.normalize();

        primitives::Vector3 intensity =
            compute_depth_of_field(origin, random_vector);

        aa_intensity = aa_intensity + intensity;

        ++k;

        bool over_threshold = false;
        for (const auto& neighbour : neighbours)
            over_threshold |=
                gradient(neighbour, aa_intensity) > options_.aa_threshold;

        if (!over_threshold)
            break;
    }

    return aa_intensity / k;
}

utils::Image Engine::run(uint height, uint width)
{
    primitives::Vector3* pixels_vector =
        scene_.camera.get_pixels_vector(height, width);
    primitives::Point3 origin = scene_.camera.get_origin();

    init_distributions(height, width);

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

            primitives::Vector3 intensity = compute_anti_aliasing(
                origin, pixels_vector[i + j * width], neighbours);

            primitives::Color pixel_color =
                primitives::Color(std::clamp(intensity.x, 0., 255.),
                                  std::clamp(intensity.y, 0., 255.),
                                  std::clamp(intensity.z, 0., 255.));

            res.pixel(i, j) = pixel_color;
        }
    }

    return res;
}

unsigned int
Engine::compute_soft_shadow(const primitives::Point3& offset_hitpoint,
                            const std::shared_ptr<scene::Light>& light,
                            const primitives::Vector3&           light_ray)
{
    if (!options_.soft_shadow_enabled)
    {
        // Take shadow into account
        // Obstacle between hitpoint and light
        if (cast_ray_light_check(offset_hitpoint, light_ray))
            return options_.nb_ray_soft_shadow;

        return 0;
    }

    int res = 0;
    for (uint k = 0; k < options_.nb_ray_soft_shadow; ++k)
    {
        primitives::Point3 jittered_light =
            light->get_center() + primitives::Point3(soft_shadow_unif_x_(re_),
                                                     soft_shadow_unif_y_(re_),
                                                     soft_shadow_unif_z_(re_));

        primitives::Vector3 random_light_ray(jittered_light - offset_hitpoint);
        random_light_ray = random_light_ray.normalize();

        // Take shadow into account
        // Obstacle between hitpoint and light
        if (cast_ray_light_check(offset_hitpoint, random_light_ray))
            ++res;
    }

    return res;
}

bool Engine::cast_ray_light_check(const primitives::Point3&  A,
                                  const primitives::Vector3& v)
{
    double min_lambda = std::numeric_limits<double>::infinity();
    std::shared_ptr<scene::Object> closest_object = nullptr;

    for (const auto& object : scene_.objects)
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
Engine::cast_ray(const primitives::Point3&  origin,
                 const primitives::Vector3& vector,
                 uint                       depth)
{
    double min_lambda = std::numeric_limits<double>::infinity();
    std::shared_ptr<scene::Object> closest_object = nullptr;

    for (const auto& object : scene_.objects)
    {
        std::optional<double> lambda = object->ray_intersection(origin, vector);
        if (lambda.has_value() && lambda.value() < min_lambda)
        {
            min_lambda     = lambda.value();
            closest_object = object;
        }
    }

    // No object in ray direction or same object that reflect in himself
    if (closest_object == nullptr ||
        (depth == 2 && last_reflected_object_ == closest_object))
        return options_.ambient_color;

    if (depth >= options_.reflection_depth)
        return primitives::Vector3();

    primitives::Point3 hitpoint =
        origin + (vector * min_lambda).get_destination();
    const scene::TextureMaterialCaracteristics& hitpoint_desc =
        closest_object->get_texture_info(hitpoint);
    primitives::Vector3 object_color(
        hitpoint_desc.color.r, hitpoint_desc.color.g, hitpoint_desc.color.b);

    primitives::Vector3 normal =
        closest_object->get_computed_normal(hitpoint, vector);
    primitives::Vector3 reflected_ray =
        (vector - normal * vector.dot(normal) * 2).normalize();
    primitives::Point3 offset_hitpoint =
        hitpoint + (normal * EPSILON).get_destination();

    primitives::Vector3 res;

    for (const auto& light : scene_.light_sources)
    {
        primitives::Vector3 cur_intensity;

        primitives::Vector3 light_ray(light->get_center() - hitpoint);
        light_ray = light_ray.normalize();

        double shadow_coef =
            compute_soft_shadow(offset_hitpoint, light, light_ray);

        primitives::Color   lc = light->get_caracteristics().color;
        primitives::Vector3 light_color(lc.r, lc.g, lc.b);

        // Add diffuse component
        cur_intensity = cur_intensity + object_color * light_color *
                                            hitpoint_desc.kd *
                                            std::max(normal.dot(light_ray), 0.);

        // Add specular component
        cur_intensity =
            cur_intensity + light_color * hitpoint_desc.ks *
                                pow(std::max(reflected_ray.dot(light_ray), 0.),
                                    hitpoint_desc.ns);

        // Apply shadow
        cur_intensity =
            cur_intensity * (1 - (shadow_coef / options_.nb_ray_soft_shadow));

        res = res + cur_intensity;
    }

    last_reflected_object_ = closest_object;

    // Add reflection
    if (options_.reflection_enabled)
    {
        primitives::Vector3 scattered_ray =
            closest_object->get_texture()->get_scattered_ray(vector, normal);

        if (typeid(closest_object->get_texture()) ==
            typeid(scene::TransparentTexture))
        {
            offset_hitpoint =
                hitpoint + (vector.normalize() * EPSILON).get_destination();
        }

        std::optional<primitives::Vector3> reflection_contribution =
            cast_ray(offset_hitpoint, scattered_ray, depth + 1);

        if (reflection_contribution.has_value())
            res = res +
                  reflection_contribution.value() * hitpoint_desc.reflection;
    }

    return res;
}
} // namespace core