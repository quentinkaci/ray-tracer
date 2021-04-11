#pragma once

#include "primitives/vector.hh"
#include "scene/objects/cube.hh"
#include "scene/objects/cube_aligned.hh"
#include "scene/objects/object.hh"

#include <memory>
#include <vector>

namespace scene
{
class EnglobingObject
{
  public:
    explicit EnglobingObject(const primitives::Point3& pos, double size);

    void add_object(const std::shared_ptr<Object>& object);

    const std::vector<std::shared_ptr<Object>>& get_objects() const;

    bool ray_intersection(const primitives::Point3&  A,
                          const primitives::Vector3& v) const;

  protected:
    const CubeAligned cube_;

    std::vector<std::shared_ptr<Object>> objects_;
};
} // namespace scene