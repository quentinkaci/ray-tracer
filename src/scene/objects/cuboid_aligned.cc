#include "scene/objects/cuboid_aligned.hh"
#include "scene/textures/image_texture.hh"

#include <cmath>

namespace scene
{
CuboidAligned::CuboidAligned(
    const std::shared_ptr<TextureMaterial>& texture_material,
    const primitives::Point3&               pos,
    const primitives::Point3&               size)
    : Object(texture_material)
    , center_pos_(pos)
    , size_(size)
    , min_pos_(pos - size)
    , max_pos_(pos + size)
{
    // FIXME: dirty trick to let ImageTexture instance know that we are using a
    // 6 faces cube texture
    if (Object::texture_material_ &&
        typeid(*Object::texture_material_) == typeid(ImageTexture))
    {
        dynamic_cast<ImageTexture*>(Object::texture_material_.get())
            ->set_cube_texture(true);
    }
}

std::optional<double>
CuboidAligned::ray_intersection(const primitives::Point3&  A,
                                const primitives::Vector3& v) const
{
    // Find intersection on x axis
    double t_min_x = (min_pos_.x - A.x) / v.x;
    double t_max_x = (max_pos_.x - A.x) / v.x;
    if (t_min_x > t_max_x)
        std::swap(t_min_x, t_max_x); // Make sure min < max

    // Find intersection on y axis
    double t_min_y = (min_pos_.y - A.y) / v.y;
    double t_max_y = (max_pos_.y - A.y) / v.y;
    if (t_min_y > t_max_y)
        std::swap(t_min_y, t_max_y); // Make sure min < max

    // Handle if ray misses axis x and/or y
    if (t_min_x > t_max_y || t_min_y > t_max_x)
        return std::nullopt;

    // Make sure min < max
    if (t_min_y > t_min_x)
        t_min_x = t_min_y;
    if (t_max_y < t_max_x)
        t_max_x = t_max_y;

    // Find intersection on z axis
    double t_min_z = (min_pos_.z - A.z) / v.z;
    double t_max_z = (max_pos_.z - A.z) / v.z;
    if (t_min_z > t_max_z)
        std::swap(t_min_z, t_max_z); // Make sure min < max

    // Handle if ray misses axis z
    if ((t_min_x > t_max_z) || (t_min_z > t_max_x))
        return std::nullopt;

    // Make sure min < max
    if (t_min_z > t_min_x)
        t_min_x = t_min_z;
    if (t_max_z < t_max_x)
        t_max_x = t_max_z;

    // If t_min_x is negative => intersection behind ray
    if (t_min_x < 0)
        return std::nullopt;

    // We return t_min_x as it is the closest distance
    return t_min_x;
}

primitives::Vector3 CuboidAligned::get_normal(const primitives::Point3& A,
                                              const primitives::Vector3&) const
{
    const primitives::Vector3 normals[6] = {primitives::Vector3(0, 0, -1),
                                            primitives::Vector3(0, 0, 1),

                                            primitives::Vector3(0, 1, 0),
                                            primitives::Vector3(0, -1, 0),

                                            primitives::Vector3(1, 0, 0),
                                            primitives::Vector3(-1, 0, 0)};

    return normals[get_cube_face(A)];
}

CUBE_FACES CuboidAligned::get_cube_face(const primitives::Point3& A) const
{
    CUBE_FACES                res   = CUBE_FACES::FRONT;
    const primitives::Vector3 point = A - center_pos_;

    double min_dist = std::numeric_limits<double>::max();

    const double distance_x = std::fabs(size_.x - std::fabs(point.x));
    if (distance_x < min_dist)
    {
        res      = (point.x < 0) ? CUBE_FACES::RIGHT : CUBE_FACES::LEFT;
        min_dist = distance_x;
    }

    const double distance_y = std::fabs(size_.y - std::fabs(point.y));
    if (distance_y < min_dist)
    {
        res      = (point.y < 0) ? CUBE_FACES::DOWN : CUBE_FACES::TOP;
        min_dist = distance_y;
    }

    const double distance_z = std::fabs(size_.z - std::fabs(point.z));
    if (distance_z < min_dist)
    {
        res      = (point.z < 0) ? CUBE_FACES::FRONT : CUBE_FACES::BACK;
        min_dist = distance_z;
    }

    return res;
}

primitives::Point3
CuboidAligned::get_planar_projection(const primitives::Point3& A) const
{
    const CUBE_FACES face = get_cube_face(A);

    primitives::Vector3 point = (A - center_pos_) / size_;

    double u = 0;
    double v = 0;

    if (face == CUBE_FACES::FRONT)
    {
        u = std::fmod(1 - point.x, 2.0) / 2.0;
        v = std::fmod(point.y + 1, 2.0) / 2.0;
    }
    else if (face == CUBE_FACES::BACK)
    {
        u = std::fmod(point.x + 1, 2.0) / 2.0;
        v = std::fmod(point.y + 1, 2.0) / 2.0;
    }
    else if (face == CUBE_FACES::TOP)
    {
        u = std::fmod(1 - point.x, 2.0) / 2.0;
        v = std::fmod(point.z + 1, 2.0) / 2.0;
    }
    else if (face == CUBE_FACES::DOWN)
    {
        u = std::fmod(1 - point.x, 2.0) / 2.0;
        v = std::fmod(1 - point.z, 2.0) / 2.0;
    }
    else if (face == CUBE_FACES::LEFT)
    {
        u = std::fmod(1 - point.z, 2.0) / 2.0;
        v = std::fmod(point.y + 1, 2.0) / 2.0;
    }
    else if (face == CUBE_FACES::RIGHT)
    {
        u = std::fmod(point.z + 1, 2.0) / 2.0;
        v = std::fmod(point.y + 1, 2.0) / 2.0;
    }

    return primitives::Point3(u, v, face);
}

TextureMaterialCaracteristics
CuboidAligned::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene