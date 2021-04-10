#pragma once

#include "core/options.hh"
#include "scene/scene.hh"

namespace utils
{
void parse_json(const std::string& filename,
                core::Options&     options,
                scene::Scene&      scene);
} // namespace utils