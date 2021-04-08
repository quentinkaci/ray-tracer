#pragma once

#include "primitives/vector.hh"

namespace core
{
struct Options
{
    // Ambient color
    primitives::Vector3 background_color =
        primitives::Vector3(102., 178., 255.);

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
    double       aperture_size  = 250.;
};
} // namespace core