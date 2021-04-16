#include "parser.hh"

#include "utils/image_png.hh"
#include "utils/image_ppm.hh"

#include "scene/blob.hh"
#include "scene/camera.hh"
#include "scene/directional_light.hh"
#include "scene/objects/billboard.hh"
#include "scene/objects/cube.hh"
#include "scene/objects/cube_aligned.hh"
#include "scene/objects/cuboid_aligned.hh"
#include "scene/objects/plane.hh"
#include "scene/objects/rectangle.hh"
#include "scene/objects/skybox.hh"
#include "scene/objects/sphere.hh"
#include "scene/point_light.hh"
#include "scene/spot_light.hh"
#include "scene/textures/bump_mapping_texture.hh"
#include "scene/textures/image_texture.hh"
#include "scene/textures/perlin_texture.hh"
#include "scene/textures/procedural_texture.hh"
#include "scene/textures/transparent_texture.hh"
#include "scene/textures/uniform_texture.hh"

#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

namespace utils
{
static primitives::Point3 parse_position(const json& j)
{
    const double x = j.at("x");
    const double y = j.at("y");
    const double z = j.at("z");

    return primitives::Point3(x, y, z);
}

static primitives::Vector3 parse_vector(const json& j)
{
    const double x = j.at("x");
    const double y = j.at("y");
    const double z = j.at("z");

    return primitives::Vector3(x, y, z);
}

static primitives::Color parse_color(const json& j)
{
    const unsigned char r = j.at("r");
    const unsigned char g = j.at("g");
    const unsigned char b = j.at("b");

    return primitives::Color(r, g, b);
}

static void parse_options(const json& j, core::Options& options)
{
    options.rendering_height          = j.at("rendering").at("height");
    options.rendering_width           = j.at("rendering").at("width");
    options.rendering_output_filename = j.at("rendering").at("output_filename");
    options.rendering_gamma = j.at("rendering").at("gamma_correction");

    if (j.contains("anaglyph"))
    {
        options.anaglyph_enabled = true;
        options.anaglyph_camera_translation =
            parse_position(j.at("anaglyph").at("camera_translation"));
    }

    if (j.contains("background"))
    {
        auto c1 = parse_color(j.at("background").at("gradient_color_top"));
        auto c2 = parse_color(j.at("background").at("gradient_color_bottom"));

        options.bg_color_top    = primitives::Vector3(c1.r, c1.g, c1.b);
        options.bg_color_bottom = primitives::Vector3(c2.r, c2.g, c2.b);
    }

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

static void parse_camera(const json& j, scene::Scene& scene)
{
    const primitives::Point3 origin = parse_position(j.at("origin"));
    const primitives::Point3 target = parse_position(j.at("target"));
    const primitives::Point3 up     = parse_position(j.at("up"));

    const double x_fov = j.at("x_fov");
    const double y_fov = j.at("y_fov");
    const double z_min = j.at("z_min");

    scene.camera = scene::Camera(origin, target, up, x_fov, y_fov, z_min);
}

static void parse_lights(const json& j, scene::Scene& scene)
{
    for (const auto& light : j)
    {
        const primitives::Color color = parse_color(light.at("color"));
        const std::string       type  = light.at("type");

        if (type == "directional")
        {
            const primitives::Vector3 direction =
                parse_vector(light.at("direction"));

            scene.light_sources.emplace_back(
                std::make_shared<scene::DirectionalLight>(color, direction));
        }
        else if (type == "spot")
        {
            const primitives::Point3 position =
                parse_position(light.at("position"));

            const primitives::Vector3 direction =
                parse_vector(light.at("direction"));

            const double angle = light.at("angle");

            scene.light_sources.emplace_back(std::make_shared<scene::SpotLight>(
                position, color, direction, angle));
        }
        else //(type == "point")
        {
            const primitives::Point3 position =
                parse_position(light.at("position"));

            scene.light_sources.emplace_back(
                std::make_shared<scene::PointLight>(position, color));
        }
    }
}

std::shared_ptr<scene::UniformTexture> parse_uniform_texture(const json& j)
{
    const double kd         = j.at("kd");
    const double ks         = j.at("ks");
    const double ns         = j.at("ns");
    const double reflection = j.at("reflection");

    const primitives::Color color = parse_color(j.at("color"));

    return std::make_shared<scene::UniformTexture>(
        scene::TextureMaterialCaracteristics{kd, ks, ns, reflection, color});
}

std::shared_ptr<scene::ImageTexture> parse_image_texture(const json& j)
{
    const double kd         = j.at("kd");
    const double ks         = j.at("ks");
    const double ns         = j.at("ns");
    const double reflection = j.at("reflection");

    double offset_x = 0.0;
    if (j.find("offset_x") != j.end())
        offset_x = j.at("offset_x");

    std::string filename = j.at("image");
    const auto  image    = create_image(filename);
    image->load(filename);

    return std::make_shared<scene::ImageTexture>(
        image,
        offset_x,
        scene::TextureMaterialCaracteristics{
            kd, ks, ns, reflection, primitives::Color()});
}

std::shared_ptr<scene::BumpMappingTexture>
parse_bump_mapping_texture(const json& j)
{
    const double kd         = j.at("kd");
    const double ks         = j.at("ks");
    const double ns         = j.at("ns");
    const double reflection = j.at("reflection");

    const primitives::Color color = parse_color(j.at("color"));

    const double octave      = j.at("octave");
    const double persistence = j.at("persistence");
    const double intensity   = j.at("intensity");

    return std::make_shared<scene::BumpMappingTexture>(
        octave,
        persistence,
        intensity,
        scene::TextureMaterialCaracteristics{kd, ks, ns, reflection, color});
}

std::shared_ptr<scene::ProceduralTexture>
parse_procedural_texture(const json& j)
{
    const double kd         = j.at("kd");
    const double ks         = j.at("ks");
    const double ns         = j.at("ns");
    const double reflection = j.at("reflection");

    const primitives::Color color = parse_color(j.at("color"));

    const std::string style = j.at("style");
    const double      size  = j.at("size");

    return std::make_shared<scene::ProceduralTexture>(
        style,
        size,
        scene::TextureMaterialCaracteristics{kd, ks, ns, reflection, color});
}

std::shared_ptr<scene::PerlinTexture> parse_perlin_texture(const json& j)
{
    const double kd         = j.at("kd");
    const double ks         = j.at("ks");
    const double ns         = j.at("ns");
    const double reflection = j.at("reflection");

    const double octave      = j.at("octave");
    const double persistence = j.at("persistence");

    const std::string        effect = j.at("effect");
    scene::PerlinTextureType type;
    if (effect == "none")
        type = scene::DEFAULT;
    else if (effect == "wood")
        type = scene::WOOD;
    else if (effect == "marble")
        type = scene::MARBLE;
    else
        throw std::logic_error(effect +
                               " is not a valid effect for perlin texture");

    std::vector<primitives::Color> colors;
    for (const auto& color : j.at("colors"))
        colors.emplace_back(parse_color(color));

    return std::make_shared<scene::PerlinTexture>(
        octave,
        persistence,
        scene::TextureMaterialCaracteristics{
            kd, ks, ns, reflection, primitives::Color()},
        type,
        colors);
}

std::shared_ptr<scene::TransparentTexture>
parse_transparent_texture(const json& j)
{
    const double kd               = j.at("kd");
    const double ks               = j.at("ks");
    const double ns               = j.at("ns");
    const double reflection       = j.at("reflection");
    const double refractive_index = j.at("refractive_index");

    const primitives::Color color = parse_color(j.at("color"));

    return std::make_shared<scene::TransparentTexture>(
        refractive_index,
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
        else if (type == "image")
            res.emplace(name, parse_image_texture(texture));
        else if (type == "bump_mapping")
            res.emplace(name, parse_bump_mapping_texture(texture));
        else if (type == "procedural")
            res.emplace(name, parse_procedural_texture(texture));
        else if (type == "perlin")
            res.emplace(name, parse_perlin_texture(texture));
        else if (type == "transparent")
            res.emplace(name, parse_transparent_texture(texture));
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
    const double             radius = j.at("radius");
    const primitives::Point3 center = parse_position(j.at("center"));

    const std::string texture = j.at("texture");

    return std::make_shared<scene::Sphere>(
        textures_map.at(texture), center, radius);
}

std::shared_ptr<scene::Skybox> parse_skybox(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
                        textures_map,
    const scene::Scene& scene)
{
    const std::string texture = j.at("texture");

    return std::make_shared<scene::Skybox>(textures_map.at(texture), scene);
}

std::shared_ptr<scene::CubeAligned> parse_cube(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    double                   size     = j.at("size");
    const primitives::Point3 position = parse_position(j.at("position"));

    const std::string texture = j.at("texture");

    return std::make_shared<scene::CubeAligned>(
        textures_map.at(texture), position, size);
}

std::shared_ptr<scene::CuboidAligned> parse_cuboid(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    const primitives::Point3 size     = parse_position(j.at("size"));
    const primitives::Point3 position = parse_position(j.at("position"));

    const std::string texture = j.at("texture");

    return std::make_shared<scene::CuboidAligned>(
        textures_map.at(texture), position, size);
}

std::shared_ptr<scene::Plane> parse_plane(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    const primitives::Vector3 normal   = parse_vector(j.at("normal"));
    const primitives::Point3  position = parse_position(j.at("position"));

    const std::string texture = j.at("texture");

    return std::make_shared<scene::Plane>(
        textures_map.at(texture), position, normal);
}

std::shared_ptr<scene::Rectangle> parse_rectangle(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    const primitives::Point3 p1 = parse_position(j.at("p1"));
    const primitives::Point3 p2 = parse_position(j.at("p2"));
    const primitives::Point3 p3 = parse_position(j.at("p3"));
    const primitives::Point3 p4 = parse_position(j.at("p4"));

    const std::string texture = j.at("texture");

    return std::make_shared<scene::Rectangle>(
        textures_map.at(texture), p1, p2, p3, p4);
}

std::shared_ptr<scene::Billboard> parse_billboard(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
                        textures_map,
    const scene::Scene& scene)
{
    const primitives::Point3 pos    = parse_position(j.at("position"));
    const double             width  = j.at("width");
    const double             height = j.at("height");

    const std::string texture = j.at("texture");

    return std::make_shared<scene::Billboard>(
        textures_map.at(texture), scene, pos, width, height);
}

std::shared_ptr<scene::Triangle> parse_triangle(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    const primitives::Point3 p1 = parse_position(j.at("p1"));
    const primitives::Point3 p2 = parse_position(j.at("p2"));
    const primitives::Point3 p3 = parse_position(j.at("p3"));

    const std::string texture = j.at("texture");

    return std::make_shared<scene::Triangle>(
        textures_map.at(texture), p1, p2, p3);
}

std::shared_ptr<scene::Blob> parse_blobs(
    const json& j,
    const std::unordered_map<std::string,
                             const std::shared_ptr<scene::TextureMaterial>>&
        textures_map)
{
    const primitives::Point3 center    = parse_position(j.at("center"));
    const std::string        texture   = j.at("texture");
    const double             delta     = j.at("delta");
    const uint               size      = j.at("size");
    const double             threshold = j.at("threshold");

    auto res = std::make_shared<scene::Blob>(
        center, delta, size, threshold, textures_map.at(texture));

    for (const auto& p : j.at("points"))
        res->add(parse_position(p));

    return res;
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
        else if (type == "skybox")
            scene.objects.emplace_back(
                parse_skybox(object, textures_map, scene));
        else if (type == "cube")
            scene.objects.emplace_back(parse_cube(object, textures_map));
        else if (type == "cuboid")
            scene.objects.emplace_back(parse_cuboid(object, textures_map));
        else if (type == "plane")
            scene.objects.emplace_back(parse_plane(object, textures_map));
        else if (type == "rectangle")
            scene.objects.emplace_back(parse_rectangle(object, textures_map));
        else if (type == "billboard")
            scene.objects.emplace_back(
                parse_billboard(object, textures_map, scene));
        else if (type == "triangle")
            scene.objects.emplace_back(parse_triangle(object, textures_map));
        else if (type == "blobs")
            parse_blobs(object, textures_map)->run(scene);
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
    if (!file.is_open())
        throw std::logic_error("Json file (" + filename +
                               ") doesn't exist.");

    json          j;
    file >> j;

    parse_options(j.at("options"), options);
    parse_scene(j.at("scene"), scene);
}
} // namespace utils