#include "blob.hh"
#include "marching_cubes.hh"
#include "scene/objects/triangle.hh"

#include <cmath>

namespace scene
{
Blob::Blob(const primitives::Point3&               origin,
           double                                  delta,
           uint                                    size,
           double                                  threshold,
           const std::shared_ptr<TextureMaterial>& texture_material)
    : origin_(origin)
    , delta_(delta)
    , size_(size)
    , threshold_(threshold)
    , texture_material_(texture_material)
{
    // Center of cube is in the middle but origin of blob is at bottom left
    primitives::Point3 cube_center(
        origin.x + size / 2, origin.y + size / 2, origin.z + size / 2);

    englobing_object_ =
        std::make_shared<EnglobingObject>(cube_center, size / 2);
}

void Blob::add(primitives::Point3 pos)
{
    if (pos.x < origin_.x || pos.x >= origin_.x + size_ || pos.y < origin_.y ||
        pos.y >= origin_.y + size_ || pos.z < origin_.z ||
        pos.z >= origin_.z + size_)
    {
        std::cout << "Potential point outside of cube" << std::endl;
        return;
    }

    energy_points_.emplace_back(pos);
}

void Blob::add_cube_segmentation(const primitives::Point3& p)
{
    // Determine corners positions
    std::vector<primitives::Point3> corners;
    corners.push_back(p - primitives::Point3(0, 0, 0));
    corners.push_back(p - primitives::Point3(delta_, 0, 0));
    corners.push_back(p - primitives::Point3(delta_, 0, delta_));
    corners.push_back(p - primitives::Point3(0, 0, delta_));
    corners.push_back(p - primitives::Point3(0, delta_, 0));
    corners.push_back(p - primitives::Point3(delta_, delta_, 0));
    corners.push_back(p - primitives::Point3(delta_, delta_, delta_));
    corners.push_back(p - primitives::Point3(0, delta_, delta_));

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
    for (int i = 0; MC_TRIANGULATION[cube_index][i] != -1; ++i)
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

        englobing_object_->add_object(
            std::make_shared<Triangle>(texture_material_, a, b, c));
    }
}

double Blob::get_potential(const primitives::Point3& point) const
{
    double res = 0;

    for (const auto& energy_point : energy_points_)
    {
        double dist = pow(point.x - energy_point.x, 2) +
                      pow(point.y - energy_point.y, 2) +
                      pow(point.z - energy_point.z, 2);

        res += 1. / dist;
    }

    return std::round(res);
}

void Blob::run(Scene& scene)
{
    double size = static_cast<double>(size_);

    for (double x = origin_.x + size; x >= origin_.x; x -= delta_)
    {
        for (double y = origin_.y + size; y >= origin_.y; y -= delta_)
        {
            for (double z = origin_.z + size; z >= origin_.z; z -= delta_)
            {
                add_cube_segmentation(primitives::Point3(x, y, z));
            }
        }
    }

    scene.englobing_objects.push_back(englobing_object_);
}
} // namespace scene