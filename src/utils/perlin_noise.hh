#pragma once

#include "types.hh"

#include <cmath>

namespace utils
{
class PerlinNoise
{
    static constexpr uint PERMUTATION[256] = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
        8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
        35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
        134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
        55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
        18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
        250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
        189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
        172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
        228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

public:
    explicit PerlinNoise();

    virtual ~PerlinNoise() = default;

    double get(double x, double y, double z) const;

    double get_octave(double x, double y, double z, uint octaves, double persistence) const;

private:
    uint permutations_[512];

    double lerp(double begin, double end, double value) const;
    double fade(double value) const;
    double grad(uint hash, double x, double y, double z) const;
};

PerlinNoise::PerlinNoise()
{
    // Duplicate the values of PERMUTATION array to avoid buffer overflow
    // when computing hash later on.
    for (uint x = 0; x < 512; x++)
        permutations_[x] = PERMUTATION[x % 256];
}

double PerlinNoise::get(double x, double y, double z) const
{
    // Implementation of Improved Noise by Ken Perlin
    // https://mrl.cs.nyu.edu/~perlin/noise/

    // We assume that x, y and z are between 0.0 and 1.0.
    // Thus we rely on their fractional part to get their actual position.
    double trash = 0;
    double xf = std::modf(x, &trash);
    double yf = std::modf(y, &trash);
    double zf = std::modf(z, &trash);

    // ------
    // Part 1
    // Find hash values for each of the 8 corners
    // ------

    uint xi = static_cast<uint>(x) % 256;
    uint yi = static_cast<uint>(y) % 256;
    uint zi = static_cast<uint>(z) % 256;

    uint aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = permutations_[permutations_[permutations_[xi] + yi] + zi];
    aba = permutations_[permutations_[permutations_[xi] + yi + 1] + zi];
    aab = permutations_[permutations_[permutations_[xi] + yi] + zi + 1];
    abb = permutations_[permutations_[permutations_[xi] + yi + 1] + zi + 1];
    baa = permutations_[permutations_[permutations_[xi + 1] + yi] + zi];
    bba = permutations_[permutations_[permutations_[xi + 1] + yi + 1] + zi];
    bab = permutations_[permutations_[permutations_[xi + 1] + yi] + zi + 1];
    bbb = permutations_[permutations_[permutations_[xi + 1] + yi + 1] + zi + 1];

    // ------
    // Part 2
    // Interpolate between the dot product of each of the 8 corners
    // ------

    double u = fade(xf);
    double v = fade(yf);
    double w = fade(zf);

    double x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
    double x2 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
    double y1 = lerp(x1, x2, v);

    x1 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
    x2 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);
    double y2 = lerp(x1, x2, v);

    return (1 + lerp(y1, y2, w)) / 2;
}

double PerlinNoise::get_octave(double x, double y, double z, uint octaves, double persistence) const
{
    double res = 0;
    double frequency = 1;
    double amplitude = 1;
    double max_value = 0;

    for (uint i = 0; i < octaves; i++)
    {
        res += get(x * frequency, y * frequency, z * frequency) * amplitude;

        max_value += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }

    return res / max_value;
}

double PerlinNoise::lerp(double begin, double end, double value) const
{
    return begin + value * (end - begin);
}

double PerlinNoise::fade(double value) const
{
    // Factorized version of this formula: 6t^5 - 15t^4 + 10t^3
    return value * value * value * (value * (value * 6 - 15) + 10);
}

double PerlinNoise::grad(uint hash, double x, double y, double z) const
{
    switch (hash & 0xF) // % 12
    {
    case 0x0:
        return x + y;
    case 0x1:
        return -x + y;
    case 0x2:
        return x - y;
    case 0x3:
        return -x - y;
    case 0x4:
        return x + z;
    case 0x5:
        return -x + z;
    case 0x6:
        return x - z;
    case 0x7:
        return -x - z;
    case 0x8:
        return y + z;
    case 0x9:
        return -y + z;
    case 0xA:
        return y - z;
    case 0xB:
        return -y - z;
    case 0xC:
        return y + x;
    case 0xD:
        return -y + z;
    case 0xE:
        return y - x;
    case 0xF:
        return -y - z;
    default:
        return 0;
    }
}
} // namespace utils