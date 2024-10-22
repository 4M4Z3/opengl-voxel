#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include "PerlinNoise.hpp"

class PerlinNoise {
public:
    PerlinNoise(unsigned int seed);
    void reseed(unsigned int newSeed);

    // Noise functions
    double noise1D(double x) const;
    double noise2D(double x, double y) const;
    double noise3D(double x, double y, double z) const;

    // Octave noise functions
    double octave1D(double x, int octaves, double persistence = 0.5) const;
    double octave2D(double x, double y, int octaves, double persistence = 0.5) const;
    double octave3D(double x, double y, double z, int octaves, double persistence = 0.5) const;

private:
    siv::PerlinNoise perlinNoise;
};

#endif // PERLIN_NOISE_H
