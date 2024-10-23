// Triangle.cpp
#include "Triangle.h"

Triangle::Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, float depth)
    : v0(v0), v1(v1), v2(v2), depth(depth) {}

Triangle::Triangle(const Triangle& other)
    : v0(other.v0), v1(other.v1), v2(other.v2), depth(other.depth) {}

Triangle::Triangle(Triangle&& other) noexcept = default;

Triangle& Triangle::operator=(Triangle&& other) noexcept = default;

void Triangle::calculateDepth(const glm::vec3& cameraPosition) {
    glm::vec3 v0Pos(v0.x, v0.y, v0.z);
    glm::vec3 v1Pos(v1.x, v1.y, v1.z);
    glm::vec3 v2Pos(v2.x, v2.y, v2.z);

    depth = (glm::distance(cameraPosition, v0Pos) +
             glm::distance(cameraPosition, v1Pos) +
             glm::distance(cameraPosition, v2Pos)) / 3.0f;
}
