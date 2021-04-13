#pragma once

#include "primitives/vector.hh"

namespace core
{
struct Options
{
    // Background gradient colors
    primitives::Vector3 bg_color_top{18, 18, 255};
    primitives::Vector3 bg_color_bottom{255, 255, 255};

    // Rendering
    unsigned int rendering_height          = 1080;
    unsigned int rendering_width           = 1920;
    std::string  rendering_output_filename = "scene.ppm";
    double       rendering_gamma           = 1.0;

    // Anaglyph effect
    bool               anaglyph_enabled            = false;
    primitives::Point3 anaglyph_camera_translation = {0.015, 0, 0};

    // Reflection
    bool         reflection_enabled = true;
    unsigned int reflection_depth   = 5;

    // Anti-aliasing
    bool         aa_enabled   = true;
    unsigned int nb_ray_aa    = 20;
    unsigned int aa_threshold = 10;

    // Soft shadow
    bool         soft_shadow_enabled   = true;
    unsigned int nb_ray_soft_shadow    = 20;
    double       min_range_soft_shadow = -1.;
    double       max_range_soft_shadow = 1.;

    // Depth of field
    bool         dof_enabled    = true;
    unsigned int nb_ray_dof     = 20;
    double       focal_distance = 10.;
    double       aperture_size  = 150.;
};
} // namespace core