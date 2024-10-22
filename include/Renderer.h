#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "World.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer(Camera& camera, World* world);
    ~Renderer();
    void render(unsigned int shaderProgram, GLFWwindow* window);

private:
    Camera& camera;
    const World* world; // Make world a const pointer
    unsigned int VAO, VBO;

    void drawTriangles(const std::vector<Vertex>& vertices);
};

#endif // RENDERER_H
