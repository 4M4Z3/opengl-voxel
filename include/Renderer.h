#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Chunk.h"
#include "World.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Vertex.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer {
    Camera &camera;
    World &world;
    std::vector<Vertex> vertices;
    unsigned int VAO, VBO;

public:
    Renderer();
    Renderer(Camera& camera, World& world);
    void drawTriangles(const std::vector<Vertex>& vertices);
    void render();
};

#endif // RENDERER_H
