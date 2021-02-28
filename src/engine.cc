#include "engine.hh"

#include <limits>
#include <optional>


namespace engine
{
    Engine::Engine(const scene::Scene& scene)
        : scene_(scene)
    {}

    utils::Image Engine::run(uint height, uint width)
    {
        primitives::Vector3* pixels_vector = scene_.camera.get_pixels_vector(height, width);
        primitives::Point3 origin = scene_.camera.get_origin();

        // for (uint i = 0; i < height * width; ++i)
        //     std::cout << pixels_vector[i] << std::endl;

        utils::Image res(height, width);

        for (uint j = 0; j < height; ++j)
        {
            for (uint i = 0; i < width; ++i)
                res.pixel(i, j) = ray_cast(origin, pixels_vector[i + j * width]);
        }

        return res;
    }

    primitives::Color Engine::ray_cast(const primitives::Point3& A, const primitives::Vector3& v)
    {
        double min_lambda = std::numeric_limits<double>::infinity();
        const scene::Object* closest_object = nullptr;

        for (const scene::Object* object : scene_.objects)
        {
            std::optional<double> lambda = object->ray_intersection(A, v);
            if (lambda.has_value() && lambda.value() < min_lambda)
            {
                min_lambda = lambda.value();
                closest_object = object;
            }
        }

        if (closest_object == nullptr)
            return primitives::Color();
        else
            return closest_object->get_texture(A + (v * min_lambda).dst).color;
    }
}