#include "parser.hh"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace utils
{
static void parse_options(const json& j, core::Options& options)
{
    options.reflection_limit = j.at("reflection_limit");

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

void parse_json(const std::string& filename,
                core::Options&     options,
                scene::Scene&)
{
    std::ifstream file(filename);
    json          j;
    file >> j;

    parse_options(j.at("options"), options);
}
} // namespace utils