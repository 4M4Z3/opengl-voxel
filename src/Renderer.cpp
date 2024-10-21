// Renderer.cpp
#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Triangle.h"
#include <iostream>
#include "Vertex.h"

Renderer::Renderer() {
    // Error if you get here!
}

Renderer::Renderer(Camera& camera, World& world) {
    this->camera = camera;
    this->world = world;

    // Initialize VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Renderer::drawTriangles(const std::vector<Vertex>& vertices) {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);                 // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::render() {
    camera.pos.z += 0.1;
    camera.pos.x += 0.1;
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); 

    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<Triangle>& triangles = camera.projectionMatrix(world);

    std::sort(triangles.begin(), triangles.end(), [](const Triangle& a, const Triangle& b) {
        return a.depth > b.depth; 
    });

    std::vector<Vertex> vertices;
    vertices.reserve(triangles.size() * 3);

    for (const Triangle& triangle : triangles) {
        vertices.push_back(triangle.v0);
        vertices.push_back(triangle.v1);
        vertices.push_back(triangle.v2);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);                 // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    camera.clearCamera();
}
