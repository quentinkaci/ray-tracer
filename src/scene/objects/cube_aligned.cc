#include "scene/objects/cube_aligned.hh"
#include "scene/textures/image_texture.hh"

#include <cmath>

namespace scene
{
CubeAligned::CubeAligned(
    const std::shared_ptr<TextureMaterial>& texture_material,
    const primitives::Point3&               pos,
    double                                  size)
    : Object(texture_material)
    , pos_(pos)
    , size_(size)
    , min_(pos.x - size, pos.y - size, pos.z - size)
    , max_(pos.x + size, pos.y + size, pos.z + size)
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
CubeAligned::ray_intersection(const primitives::Point3&  A,
                              const primitives::Vector3& v) const
{
    double tmin = (min_.x - A.x) / v.x;
    double tmax = (max_.x - A.x) / v.x;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    double tymin = (min_.y - A.y) / v.y;
    double tymax = (max_.y - A.y) / v.y;

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return std::nullopt;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    double tzmin = (min_.z - A.z) / v.z;
    double tzmax = (max_.z - A.z) / v.z;

    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return std::nullopt;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    if (tmin >= 0)
        return tmin;

    return std::nullopt;
}

primitives::Vector3 CubeAligned::get_normal(const primitives::Point3& A,
                                            const primitives::Vector3&) const
{
    CUBE_FACES face = get_cube_face(A);

    primitives::Vector3 normals[6] = {primitives::Vector3(0, 0, -1),
                                      primitives::Vector3(0, 0, 1),

                                      primitives::Vector3(0, 1, 0),
                                      primitives::Vector3(0, -1, 0),

                                      primitives::Vector3(1, 0, 0),
                                      primitives::Vector3(-1, 0, 0)};

    return normals[face];
}

CUBE_FACES CubeAligned::get_cube_face(const primitives::Point3& A) const
{
    CUBE_FACES          res   = CUBE_FACES::FRONT;
    primitives::Vector3 point = A - pos_;

    float min = std::numeric_limits<float>::max();

    float distance = std::abs(size_ - std::abs(point.x));
    if (distance < min)
    {
        min = distance;
        res = (point.x < 0) ? CUBE_FACES::RIGHT : CUBE_FACES::LEFT;
    }

    distance = std::abs(size_ - std::abs(point.y));
    if (distance < min)
    {
        min = distance;
        res = (point.y < 0) ? CUBE_FACES::DOWN : CUBE_FACES::TOP;
    }

    distance = std::abs(size_ - std::abs(point.z));
    if (distance < min)
    {
        min = distance;
        res = (point.z < 0) ? CUBE_FACES::FRONT : CUBE_FACES::BACK;
    }

    return res;
}

primitives::Point3
CubeAligned::get_planar_projection(const primitives::Point3& A) const
{
    CUBE_FACES face = get_cube_face(A);

    const primitives::Point3& point = (A - pos_) / size_;

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
CubeAligned::get_texture_info(const primitives::Point3& A) const
{
    return Object::texture_material_->get_caracteristics(
        get_planar_projection(A));
}
} // namespace scene