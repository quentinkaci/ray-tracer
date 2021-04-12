#pragma once

#include "primitives/vector.hh"

#include <vector>

namespace scene
{
class Camera
{
  public:
    // All angles are supposed to be in degree

    Camera() = default;

    Camera(const primitives::Point3& origin,
           const primitives::Point3& target,
           const primitives::Point3& up,
           double                    x_fov,
           double                    y_fov,
           double                    z_min);

    primitives::Point3 get_origin() const;

    primitives::Point3 get_target() const;

    double get_unit_x(uint width) const;

    double get_unit_y(uint height) const;

    std::vector<primitives::Vector3> get_pixels_vector(uint height,
                                                       uint width) const;

  private:
    primitives::Point3 origin_ = {0, 0, 0};

    primitives::Point3 target_ = {0, 0, 1};

    primitives::Vector3 up_ = {0, 1, 0};

    double x_fov_ = 62.22;

    double y_fov_ = 35;

    double z_min_ = 1;
};
} // namespace scene