// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Vertex.h"
#include "Triangle.h"
#include "World.h"
#include "Camera.h"
#include <GL/glew.h>

class Renderer {
public:
    Renderer(Camera& camera, World& world);
    void render();
    void drawTriangles(const std::vector<Vertex>& vertices);

private:
    GLuint VAO, VBO;
    Camera& camera;
    World& world;
};

#endif // RENDERER_H
