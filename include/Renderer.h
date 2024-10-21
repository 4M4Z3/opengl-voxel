// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Camera.h"
#include "World.h"
#include "Vertex.h"
#include <GL/glew.h>

class Renderer {
public:
    Renderer();
    Renderer(Camera& camera, World& world);
    void render();
    void drawTriangles(const std::vector<Vertex>& vertices);

private:
    GLuint VAO, VBO, textureID;
    Camera camera;
    World world;
    GLuint loadTexture(const char* filePath);
};

#endif // RENDERER_H
