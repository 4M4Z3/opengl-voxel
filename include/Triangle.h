// Triangle.h
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vertex.h"
#include <glm/glm.hpp>

class Triangle {
public:
    Vertex v0, v1, v2;
    float depth;

    Triangle() = default;
    Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, float depth);
    Triangle(const Triangle& other);
    Triangle(Triangle&& other) noexcept;
    Triangle& operator=(Triangle&& other) noexcept;

    void calculateDepth(const glm::vec3& cameraPosition);
};

#endif // TRIANGLE_H
