#pragma once

#include <vector>

namespace scene
{
    struct Scene
    {
        Scene() = default;

        ~Scene() = default;

        // FIXME
        std::vector<int> objects;
        std::vector<int> light_sources;
        int camera;
    };
}