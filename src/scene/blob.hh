#pragma once

#include "scene/scene.hh"

#include <tuple>
#include <vector>

namespace scene
{
    class Blob
    {
    public:
        explicit Blob(const primitives::Point3& origin, double delta, uint size, double threshold, const TextureMaterial& texture_material);

        virtual ~Blob() = default;

        void add(primitives::Point3 pos, double value);

        void run(Scene& scene) const;

    private:
        double get_potential(const primitives::Point3& point) const;

        void add_cube_segmentation(Scene& scene, const primitives::Point3& p) const;

        std::vector<std::tuple<primitives::Point3, double>> energy_points_;

        primitives::Point3 origin_;

        double delta_;

        uint size_;

        double threshold_;

        const TextureMaterial& texture_material_;
    };
}