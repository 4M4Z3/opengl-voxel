#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Block.h"

Renderer::Renderer(Camera& camera, World* world) // Accept World as a pointer
    : camera(camera), world(world) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Renderer::drawTriangles(const std::vector<Vertex>& vertices) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute (2 floats)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Ambient Occlusion attribute (1 float)
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::render(unsigned int shaderProgram, GLFWwindow* window) {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspectRatio = static_cast<float>(width) / height;

    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix(aspectRatio);

    glUseProgram(shaderProgram);

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    std::vector<Triangle> sortedTriangles = world->getVisibleTriangles(camera);
    std::sort(sortedTriangles.begin(), sortedTriangles.end(), [](const Triangle& a, const Triangle& b) {
        return a.depth > b.depth;
    });

    std::vector<Vertex> vertices;
    vertices.reserve(sortedTriangles.size() * 3);
    for (const Triangle& triangle : sortedTriangles) {
        vertices.push_back(triangle.v0);
        vertices.push_back(triangle.v1);
        vertices.push_back(triangle.v2);
    }

    // Draw the triangles
    drawTriangles(vertices);

    // Bind and draw the VAO
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
