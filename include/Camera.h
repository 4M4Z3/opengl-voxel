// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <cmath>
#include <iostream>
#include "Triangle.h"
#include "Vec3.h"
#include "World.h"
#include "Vertex.h"

class Camera {
public:
    Vec3 pos; 
    Vec3 rot; 
    float fov; 

    // Direction vectors
    Vec3 forward;
    Vec3 right;
    Vec3 up;

    std::vector<Triangle> view_triangles; 

    Camera();
    Camera(Vec3 pos, Vec3 rot, float fov);

    // Methods
    void updateCameraVectors();
    std::vector<Triangle>& projectionMatrix(World& world);
    void printViewTriangles(const std::vector<Triangle>& view_triangles); 
    void clearCamera(); 

private:
    float toRadians(float degrees); 
};

#endif
