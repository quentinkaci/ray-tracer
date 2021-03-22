#include <algorithm>
#include <random>

#include "perlin_noise.hh"

namespace utils
{
PerlinNoise::PerlinNoise()
{
    // Duplicate the values to avoid buffer overflow
    // when computing hash later on.
    for (uint x = 0; x < 512; x++)
        hash_.push_back(x % 256);

    // Shuffle hash table values
    std::shuffle(
        hash_.begin(), hash_.end(), std::default_random_engine(PERLIN_SEED));
}

double PerlinNoise::get(double x, double y, double z) const
{
    // Implementation of Improved Noise by Ken Perlin
    // https://mrl.cs.nyu.edu/~perlin/noise/

    // We assume that x, y and z are between 0.0 and 1.0.
    // Thus we rely on their fractional part to get their actual position.
    double trash = 0;
    double xf    = std::modf(x, &trash);
    double yf    = std::modf(y, &trash);
    double zf    = std::modf(z, &trash);

    // ------
    // Part 1
    // Find hash values for each of the 8 corners of the cube
    // ------

    uint xi = static_cast<uint>(x) % 256;
    uint yi = static_cast<uint>(y) % 256;
    uint zi = static_cast<uint>(z) % 256;

    uint aaa = hash_[hash_[hash_[xi] + yi] + zi];
    uint aba = hash_[hash_[hash_[xi] + yi + 1] + zi];
    uint aab = hash_[hash_[hash_[xi] + yi] + zi + 1];
    uint abb = hash_[hash_[hash_[xi] + yi + 1] + zi + 1];
    uint baa = hash_[hash_[hash_[xi + 1] + yi] + zi];
    uint bba = hash_[hash_[hash_[xi + 1] + yi + 1] + zi];
    uint bab = hash_[hash_[hash_[xi + 1] + yi] + zi + 1];
    uint bbb = hash_[hash_[hash_[xi + 1] + yi + 1] + zi + 1];

    // ------
    // Part 2
    // Interpolate between the dot product of each of the 8 corners
    // ------

    double u = fade(xf);
    double v = fade(yf);
    double w = fade(zf);

    double aaa_grad = grad(aaa, xf, yf, zf);
    double baa_grad = grad(baa, xf - 1, yf, zf);
    double x1       = lerp(aaa_grad, baa_grad, u);

    double aba_grad = grad(aba, xf, yf - 1, zf);
    double bba_grad = grad(bba, xf - 1, yf - 1, zf);
    double x2       = lerp(aba_grad, bba_grad, u);

    double y1 = lerp(x1, x2, v);

    double aab_grad = grad(aab, xf, yf, zf - 1);
    double bab_grad = grad(bab, xf - 1, yf, zf - 1);
    x1              = lerp(aab_grad, bab_grad, u);

    double abb_grad = grad(abb, xf, yf - 1, zf - 1);
    double bbb_grad = grad(bbb, xf - 1, yf - 1, zf - 1);
    x2              = lerp(abb_grad, bbb_grad, u);

    double y2 = lerp(x1, x2, v);

    return (1 + lerp(y1, y2, w)) / 2;
}

double PerlinNoise::get_octave(double x,
                               double y,
                               double z,
                               uint   octaves,
                               double persistence) const
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
    // Depending on hash value, compute the dot product between vector
    // (x, y, z) and a unit vector (eg: (1, -1, 0)).

    switch (hash & 0xF)
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