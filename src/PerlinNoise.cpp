#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(unsigned int seed)
    : perlinNoise(seed) {}

void PerlinNoise::reseed(unsigned int newSeed) {
    perlinNoise.reseed(newSeed);
}

double PerlinNoise::noise1D(double x) const {
    return perlinNoise.noise1D(x);
}

double PerlinNoise::noise2D(double x, double y) const {
    return perlinNoise.noise2D(x, y);
}

double PerlinNoise::noise3D(double x, double y, double z) const {
    return perlinNoise.noise3D(x, y, z);
}

double PerlinNoise::octave1D(double x, int octaves, double persistence) const {
    return perlinNoise.octave1D(x, octaves, persistence);
}

double PerlinNoise::octave2D(double x, double y, int octaves, double persistence) const {
    return perlinNoise.octave2D(x, y, octaves, persistence);
}

double PerlinNoise::octave3D(double x, double y, double z, int octaves, double persistence) const {
    return perlinNoise.octave3D(x, y, z, octaves, persistence);
}
