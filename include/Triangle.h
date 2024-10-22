// Triangle.h
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vertex.h"

class Triangle {
public:
    Vertex v0, v1, v2;
    float depth;

    Triangle() = default;
    Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, float depth)
        : v0(v0), v1(v1), v2(v2), depth(depth) {}

    // Explicitly define a copy constructor
    Triangle(const Triangle& other)
        : v0(other.v0), v1(other.v1), v2(other.v2), depth(other.depth) {}

    // Add move constructor and move assignment operator
    Triangle(Triangle&& other) noexcept = default;
    Triangle& operator=(Triangle&& other) noexcept = default;
};

#endif // TRIANGLE_H
