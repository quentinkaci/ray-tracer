#include "scene/objects/englobing_object.hh"

#include <typeinfo>
#include <utility>

namespace scene
{
EnglobingObject::EnglobingObject(const primitives::Point3& pos, double size)
    : cube_(nullptr, pos, size)
{
}

void EnglobingObject::add_object(const std::shared_ptr<Object>& object)
{
    objects_.emplace_back(object);
}

const std::vector<std::shared_ptr<Object>>& EnglobingObject::get_objects() const
{
    return objects_;
}

bool EnglobingObject::ray_intersection(const primitives::Point3&  A,
                                       const primitives::Vector3& v) const
{
    return cube_.ray_intersection(A, v).has_value();
}

} // namespace scene