#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Perlin{

private:
    struct Vector2
    {
        float x,y;
        Vector2(float newX, float newY): x(newX), y(newY) {}
        Vector2() : x(0), y(0) {}
    };
    int gridWidth;
    int gridHeight;
    Vector2 arrayOfVectors[8];

public:
    float SamplePerlin(float x, float y);
    float DotGridGradient(int ix, int iy, float x, float y);
    Vector2 GenerateRandomGradient();
    float LinearInterpolation(float a, float b, float t);
    float CubicInterpolation(float a, float b, float c);
    Vector2 randomGradient(int ix, int iy);
};