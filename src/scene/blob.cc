#include "blob.hh"
#include "marching_cubes.hh"
#include "triangle.hh"

#include <cmath>

namespace scene
{
    Blob::Blob(const primitives::Point3& origin, double delta, uint size, double threshold, const TextureMaterial& texture_material)
        : origin_(origin)
        , delta_(delta)
        , size_(size)
        , threshold_(threshold)
        , texture_material_(texture_material)
    {}

    void Blob::add(primitives::Point3 pos, double value)
    {
        primitives::Point3 relative_pos = pos - origin_;

        if (relative_pos.x < 0 || relative_pos.x >= size_
            || relative_pos.y <  0 || relative_pos.y >= size_
            || relative_pos.z <  0 || relative_pos.z >= size_)
        {
            std::cout << "MERDE" << std::endl;
            return;
        }

        double range = delta_ / size_;
        relative_pos.x = std::floor(relative_pos.x * range);
        relative_pos.y = std::floor(relative_pos.y * range);
        relative_pos.z = std::floor(relative_pos.z * range);

        energy_points_.emplace_back(std::make_tuple<>(relative_pos, value));
    }

    void Blob::add_cube_segmentation(Scene& scene, const primitives::Point3& p) const
    {
        // Determine corners positions
        std::vector<primitives::Point3> corners;
        corners.push_back(p + primitives::Point3(0, 0, 0));
        corners.push_back(p + primitives::Point3(delta_, 0, 0));
        corners.push_back(p + primitives::Point3(delta_, 0, delta_));
        corners.push_back(p + primitives::Point3(0, 0, delta_));
        corners.push_back(p + primitives::Point3(0, delta_, 0));
        corners.push_back(p + primitives::Point3(delta_, delta_, 0));
        corners.push_back(p + primitives::Point3(delta_, delta_, delta_));
        corners.push_back(p + primitives::Point3(0, delta_, delta_));

        // Determine configuration index
        int cube_index = 0;
        for (int i = 0; i < 8; i++)
        {
            double potential = get_potential(corners[i]);
            if (potential < threshold_)
                cube_index |= 1 << i;
        }

        // Create vertices
        std::vector<primitives::Point3> vertices;
        for (int i = 0; MC_TRIANGULATION[cube_index][i] != -1; i++)
        {
            // Lookup in triangulation table
            int edge_index = MC_TRIANGULATION[cube_index][i];

            // Determine indices of corner points making up the edge
            int index_A = MC_CORNER_INDEX_A_FROM_EDGE[edge_index];
            int index_B = MC_CORNER_INDEX_B_FROM_EDGE[edge_index];

            // Find middle of edge
            primitives::Point3 vertex = (corners[index_A] + corners[index_B]) / 2;
            vertices.push_back(vertex);
        }

        // Add triangles to scene
        for (uint i = 0; i < vertices.size(); i += 3)
        {
            primitives::Point3 a = vertices[i];
            primitives::Point3 b = vertices[i + 1];
            primitives::Point3 c = vertices[i + 2];
            scene.objects.emplace_back(new Triangle(texture_material_, a, b, c));
        }
    }

    double Blob::get_potential(const primitives::Point3& point) const
    {
        double res = 0;

        for (const auto& energy_point : energy_points_)
        {
            primitives::Point3 energy_source = std::get<0>(energy_point);
            res += std::get<1>(energy_point)
                / (pow(energy_source.x - point.x, 2) + pow(energy_source.y - point.y, 2) + pow(energy_source.z - point.z, 2));
        }

        return res;
    }

    void Blob::run(Scene& scene) const
    {
        double size = static_cast<double>(size_);

        for (double x = 0; x < size; x += delta_)
        {
            for (double y = 0; y < size; y += delta_)
            {
                for (double z = 0; z < size; z += delta_)
                {
                    add_cube_segmentation(scene, primitives::Point3(x, y, z) - origin_);
                }
            }
        }

        std::cout << "YOUPI 2"  << std::endl;
    }
}