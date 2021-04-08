#include "parser.hh"

#include "scene/camera.hh"
#include "scene/point_light.hh"
#include "scene/sphere.hh"
#include "scene/uniform_texture.hh"

#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

namespace utils
{
static void parse_options(const json& j, core::Options& options)
{
    options.rendering_height          = j.at("rendering").at("height");
    options.rendering_width           = j.at("rendering").at("width");
    options.rendering_output_filename = j.at("rendering").at("output_filename");

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

static void parse_lights(const json& j, scene::Scene& scene)
{
    for (const auto& light : j)
    {
        primitives::Point3 position = parse_position(light.at("position"));
        primitives::Color  color    = parse_color(light.at("color"));

        scene.light_sources.emplace_back(
            std::make_shared<scene::PointLight>(position, color));
    }
}

std::shared_ptr<scene::UniformTexture> parse_uniform_texture(const json& j)
{
    double kd         = j.at("kd");
    double ks         = j.at("ks");
    double ns         = j.at("ns");
    double reflection = j.at("reflection");

    primitives::Color color = parse_color(j.at("color"));

    return std::make_shared<scene::UniformTexture>(
        scene::TextureMaterialCaracteristics{kd, ks, ns, reflection, color});
}

static std::unordered_map<std::string,
                          const std::shared_ptr<scene::TextureMaterial>>
parse_textures(const json& j)
{
    std::unordered_map<std::string,
                       const std::shared_ptr<scene::TextureMaterial>>
        res;

    for (const auto& texture : j)
    {
        std::string name = texture.at("name");

        std::string type = texture.at("type");
        if (type == "uniform")
            res.emplace(name, parse_uniform_texture(texture));
        else
            throw std::logic_error(type + " is an invalid texture type");
    }

    return res;
}

std::shared_ptr<scene::Sphere> parse_sphere(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    double             radius = j.at("radius");
    primitives::Point3 center = parse_position(j.at("center"));

    std::string texture = j.at("texture");

    return std::make_shared<scene::Sphere>(
        *textures_map.at(texture), center, radius);
}

static void parse_objects(
    const json&   j,
    scene::Scene& scene,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    for (const auto& object : j)
    {
        std::string type = object.at("type");
        if (type == "sphere")
            scene.objects.emplace_back(parse_sphere(object, textures_map));
        else
            throw std::logic_error(type + " is an invalid object type");
    }
}

static void parse_scene(const json& j, scene::Scene& scene)
{
    parse_camera(j.at("camera"), scene);
    parse_lights(j.at("lights"), scene);

    auto textures_map = parse_textures(j.at("textures"));
    parse_objects(j.at("objects"), scene, textures_map);
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