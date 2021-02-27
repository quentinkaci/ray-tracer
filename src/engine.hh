#pragma once

#include "utils/image.hh"
#include "scene/scene.hh"
#include "primitives/color.hh"

namespace engine
{
    class Engine
    {
    public:
        Engine(const scene::Scene& scene);

        utils::Image run(uint height, uint width);

    private:
        primitives::Color ray_cast(const primitives::Point3& A, const primitives::Vector3& v);

        scene::Scene scene_;
    };
}