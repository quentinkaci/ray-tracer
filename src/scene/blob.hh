#pragma once

#include "scene/scene.hh"

#include <tuple>
#include <vector>

namespace scene
{
class Blob
{
  public:
    explicit Blob(const primitives::Point3&               origin,
                  double                                  delta,
                  uint                                    size,
                  double                                  threshold,
                  const std::shared_ptr<TextureMaterial>& texture_material);

    virtual ~Blob() = default;

    void add(primitives::Point3 pos);

    void run(Scene& scene);

  private:
    double get_potential(const primitives::Point3& point) const;

    void add_cube_segmentation(const primitives::Point3& p);

    std::vector<primitives::Point3> energy_points_;

    primitives::Point3 origin_;

    double delta_;

    uint size_;

    double threshold_;

    const std::shared_ptr<TextureMaterial>& texture_material_;

    std::shared_ptr<EnglobingObject> englobing_object_;
};
} // namespace scene