// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <iostream>
#include <sstream>
#include "Vertex.h"
#include "Camera.h"
#include "World.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int loadTexture(const char* filepath) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

bool initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

GLFWwindow* createWindow(int width, int height, const char* title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glEnable(GL_MULTISAMPLE);

    return window;
}

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Error handling
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int createShaderProgram() {
    const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;   // Position
layout(location = 1) in vec3 aColor; // Color
layout(location = 2) in vec2 aTexCoord; // Texture coordinates

out vec3 ourColor;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}

    )";

    const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    vec4 textureColor = texture(ourTexture, TexCoord);
    FragColor = textureColor * vec4(ourColor, 1.0);
}

    )";

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Error handling
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error: Shader program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    if (!initOpenGL()) {
        return -1;
    }

    GLFWwindow* window = createWindow(1000, 800, "Voxel Game");
    if (!window) {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Camera camera;
    World world;

    Renderer renderer(camera, world);

    unsigned int shaderProgram = createShaderProgram();
    unsigned int textureID = loadTexture("assets/atlas.png"); // Load the texture atlas

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the active shader program
        glUseProgram(shaderProgram);

        // Set the texture uniform and bind the texture
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0); // Set the texture uniform to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Render the scene
        renderer.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


