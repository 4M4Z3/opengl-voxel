// Camera.cpp
#include <vector>
#include <cmath>
#include <iostream>
#include "Camera.h"
#include "World.h"
#include "Vertex.h"

Camera::Camera() {
    pos = {50, 30, 50};
    rot = {-50, 135, 0};
    fov = 120;
}

Camera::Camera(Vec3 pos, Vec3 rot, float fov) {
    this->pos = pos;
    this->rot = rot;
    this->fov = fov;
}

inline float Camera::toRadians(float degrees) {
    return degrees * M_PI / 180.0f;
}

std::vector<Triangle>& Camera::projectionMatrix(World& world) {
    float pitchRad = toRadians(rot.x);
    float yawRad = toRadians(rot.y);
    float rollRad = toRadians(rot.z);

    float cosPitch = cos(pitchRad);
    float sinPitch = sin(pitchRad);
    float cosYaw = cos(yawRad);
    float sinYaw = sin(yawRad);
    float cosRoll = cos(rollRad);
    float sinRoll = sin(rollRad);

    float aspectRatio = 1000.0f / 800.0f;
    float fovRad = toRadians(fov);
    float tanHalfFov = tan(fovRad / 2.0f);

    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    view_triangles.clear();

    for (auto& [key, chunk] : world.chunks) {
        std::vector<Triangle>& chunkTriangles = chunk.triangles;

        for (const Triangle& triangle : chunkTriangles) {
            Triangle transformedTriangle;

            float centroidX = (triangle.v0.x + triangle.v1.x + triangle.v2.x) / 3.0f;
            float centroidY = (triangle.v0.y + triangle.v1.y + triangle.v2.y) / 3.0f;
            float centroidZ = (triangle.v0.z + triangle.v1.z + triangle.v2.z) / 3.0f;

            float dx = centroidX - pos.x;
            float dy = centroidY - pos.y;
            float dz = centroidZ - pos.z;
            float depth = sqrt(dx * dx + dy * dy + dz * dz);

            for (int k = 0; k < 3; ++k) {
                const Vertex& vertex = (k == 0) ? triangle.v0 : (k == 1) ? triangle.v1 : triangle.v2;

                float translatedX = vertex.x - pos.x;
                float translatedY = vertex.y - pos.y;
                float translatedZ = vertex.z - pos.z;

                float rotatedX = cosYaw * translatedX + sinYaw * translatedZ;
                float rotatedZ = -sinYaw * translatedX + cosYaw * translatedZ;

                float rotatedY = cosPitch * translatedY - sinPitch * rotatedZ;
                rotatedZ = sinPitch * translatedY + cosPitch * rotatedZ;

                float finalX = cosRoll * rotatedX - sinRoll * rotatedY;
                float finalY = sinRoll * rotatedX + cosRoll * rotatedY;

                if (rotatedZ < nearPlane || rotatedZ > farPlane) {
                    continue;
                }

                float perspectiveX = finalX / (rotatedZ * tanHalfFov * aspectRatio);
                float perspectiveY = finalY / (rotatedZ * tanHalfFov);

                perspectiveX = (perspectiveX + 1.0f) / 2.0f * 2.0f - 1.0f;
                perspectiveY = (perspectiveY + 1.0f) / 2.0f * 2.0f - 1.0f;

                float zNormalized = (rotatedZ - nearPlane) / (farPlane - nearPlane);
                float perspectiveZ = zNormalized * 2.0f - 1.0f;                     

                if (k == 0) {
                    transformedTriangle.v0 = {perspectiveX, perspectiveY, perspectiveZ, vertex.r, vertex.g, vertex.b};
                } else if (k == 1) {
                    transformedTriangle.v1 = {perspectiveX, perspectiveY, perspectiveZ, vertex.r, vertex.g, vertex.b};
                } else {
                    transformedTriangle.v2 = {perspectiveX, perspectiveY, perspectiveZ, vertex.r, vertex.g, vertex.b};
                }
            }

            transformedTriangle.depth = depth;

            view_triangles.push_back(transformedTriangle);
        }
    }
    return view_triangles;
}

void Camera::printViewTriangles(const std::vector<Triangle>& view_triangles) {
    for (size_t i = 0; i < view_triangles.size(); ++i) {
        const Triangle& triangle = view_triangles[i];
        std::cout << "Triangle " << i << ":\n";

        const Vertex vertices[3] = {triangle.v0, triangle.v1, triangle.v2};
        for (size_t j = 0; j < 3; ++j) {
            const Vertex& v = vertices[j];
            std::cout << "v" << j << ": "
                      << v.x << " " << v.y << " " << v.z << " "
                      << v.r << " " << v.g << " " << v.b << "\n";
        }
        std::cout << std::endl;
    }
}

void Camera::clearCamera() {
    view_triangles.clear();
}
