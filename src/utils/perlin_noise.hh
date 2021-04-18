#pragma once

#include "types.hh"

#include <cmath>
#include <vector>

namespace utils
{
class PerlinNoise
{
    static constexpr uint PERLIN_SEED = 1717;

  public:
    explicit PerlinNoise();

    virtual ~PerlinNoise() = default;

    double get(double x, double y, double z) const;

    double get_octave(double x,
                      double y,
                      double z,
                      uint   octaves,
                      double persistence) const;

  private:
    std::vector<uint> hash_;

    double lerp(double begin, double end, double value) const;
    double fade(double value) const;
    double dot(uint hash, double x, double y, double z) const;
};
} // namespace utils