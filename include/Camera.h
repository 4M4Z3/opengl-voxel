// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Triangle.h"
#include "World.h"
#include <vector>

class Camera {
public:
    Vec3 pos;
    Vec3 rot;
    float fov;

    Camera();
    Camera(Vec3 pos, Vec3 rot, float fov);

    std::vector<Triangle>& projectionMatrix(World& world);
    void clearCamera();

private:
    std::vector<Triangle> view_triangles;
    inline float toRadians(float degrees);
    void printViewTriangles(const std::vector<Triangle>& view_triangles);
};

#endif // CAMERA_H
