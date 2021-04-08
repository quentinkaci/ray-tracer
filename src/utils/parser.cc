#include "parser.hh"

#include "scene/camera.hh"
#include "scene/point_light.hh"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace utils
{
static void parse_options(const json& j, core::Options& options)
{
    if (!j.contains("reflection"))
        options.reflection_enabled = false;
    else
    {
        options.reflection_depth = j.at("reflection").at("depth");
        if (options.reflection_depth <= 0)
            throw std::logic_error("Reflection depth must be greater than 0");
    }

    if (!j.contains("anti_aliasing"))
        options.aa_enabled = false;
    else
    {
        options.nb_ray_aa    = j.at("anti_aliasing").at("nb_ray");
        options.aa_threshold = j.at("anti_aliasing").at("threshold");
    }

    if (!j.contains("soft_shadow"))
        options.soft_shadow_enabled = false;
    else
    {
        options.nb_ray_soft_shadow    = j.at("soft_shadow").at("nb_ray");
        options.min_range_soft_shadow = j.at("soft_shadow").at("min_range");
        options.max_range_soft_shadow = j.at("soft_shadow").at("max_range");
    }

    if (!j.contains("depth_of_field"))
        options.dof_enabled = false;
    else
    {
        options.nb_ray_dof     = j.at("depth_of_field").at("nb_ray");
        options.focal_distance = j.at("depth_of_field").at("focal_distance");
        options.aperture_size  = j.at("depth_of_field").at("aperture_size");
    }
}

static primitives::Point3 parse_position(const json& j)
{
    double x = j.at("x");
    double y = j.at("y");
    double z = j.at("z");

    return primitives::Point3(x, y, z);
}

static primitives::Color parse_color(const json& j)
{
    unsigned char r = j.at("r");
    unsigned char g = j.at("g");
    unsigned char b = j.at("b");

    return primitives::Color(r, g, b);
}

static void parse_lights(const json& j, scene::Scene& scene)
{
    for (const auto& light : j)
    {
        primitives::Point3 position = parse_position(light.at("position"));
        primitives::Color  color    = parse_color(light.at("color"));

        scene.light_sources.emplace_back(
            new scene::PointLight(position, color));
    }
}

static void parse_camera(const json& j, scene::Scene& scene)
{
    primitives::Point3 origin = parse_position(j.at("origin"));
    primitives::Point3 target = parse_position(j.at("target"));
    primitives::Point3 up     = parse_position(j.at("up"));

    double x_fov = j.at("x_fov");
    double y_fov = j.at("y_fov");
    double z_min = j.at("z_min");

    scene.camera = scene::Camera(origin, target, up, x_fov, y_fov, z_min);
}

static void parse_scene(const json& j, scene::Scene& scene)
{
    parse_camera(j.at("camera"), scene);
    parse_lights(j.at("lights"), scene);
}

void parse_json(const std::string& filename,
                core::Options&     options,
                scene::Scene&      scene)
{
    std::ifstream file(filename);
    json          j;
    file >> j;

    parse_options(j.at("options"), options);
    parse_scene(j.at("scene"), scene);
}
} // namespace utils