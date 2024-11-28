#include "Perlin.h"
#include <iostream>
#include <random>

// sample perlin noise at coordinates (x,y)
float Perlin::SamplePerlin(float x, float y)
{
    // find nearest grid points
    int x0 = int(x);
    int y0 = int(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // compute interpolation weights
    float InterWeightX = x - (float)x0;
    float InterWeightY = y - (float)y0;

    // interpoalte top corners
    float n0 = DotGridGradient(x0, y0, x, y);
    float n3 = DotGridGradient(x1, y0, x, y);
    // interpolate bottom corners
    float n1 = DotGridGradient(x0, y1, x, y);
    float n2 = DotGridGradient(x1, y1, x, y);
    
    float interpolX = CubicInterpolation(n0, n3, InterWeightX);
    float interpolY = CubicInterpolation(n1, n2, InterWeightX);

    // interpolate in y
    float value = CubicInterpolation(interpolX, interpolY, InterWeightY);
    return value;
}

// compute the dot product of the distance and gradient vectors
float Perlin::DotGridGradient(int cornerX, int cornerY, float x, float y)
{
    // find random gradient vector at the corner
    Vector2 gradient = randomGradient(cornerX, cornerY);

    //  get the distance between the desired pixel and one of the rectangle corners
    float dx = x - (float)(cornerX);
    float dy = y - (float)(cornerY);

    // compute and return the dot product
    return (dx * gradient.x + dy * gradient.y);
}

Perlin::Vector2 Perlin::GenerateRandomGradient()
{
    // delivers bad result
    return arrayOfVectors[rand() % 8];
    // seed the random generator
    //     std::srand(static_cast<unsigned>(std::time(nullptr)));
    //     // pick random number -> normalize value -> scale to full circle -> value in radians
    //     float angle = (float)(std::rand()) / RAND_MAX * 2.0f * 3.14159265359f;
    //     return Vector2(std::cos(angle), std::sin(angle));
}

Perlin::Vector2 Perlin::randomGradient(int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1));

    // Create the vector from the angle
    Vector2 v(sin(random),cos(random));

    return v;
}

float Perlin::LinearInterpolation(float a, float b, float t)
{
    return a + t * (b - a);
}

float Perlin::CubicInterpolation(float a, float b, float c)
{
    return (b - a) * (3.0 - c * 2.0) * c * c + a;
}

