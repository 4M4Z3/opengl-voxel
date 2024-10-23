// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <iostream>
#include <unordered_map>
#include "Vertex.h"
#include "TextureMap.h"
#include "Player.h"
#include "World.h"
#include "Movement.h"
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    glEnable(GL_DEPTH_TEST);

    return window;
}

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

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
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        layout(location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * vec4(aPos, 1.0);
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

bool windowFocused = true;
std::unordered_map<int, bool> keyStates;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
    } else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}

void windowFocusCallback(GLFWwindow* window, int focused) {
    windowFocused = focused;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Movement* movement = static_cast<Movement*>(glfwGetWindowUserPointer(window));
    if (!windowFocused || !movement) {
        return;
    }

    double dx = xpos - 1100 / 2;
    double dy = ypos - 800 / 2;

    if (dx > 0) {
        movement->lookRight(dx);
    } else if (dx < 0) {
        movement->lookLeft(-dx);
    }

    if (dy > 0) {
        movement->lookUp(dy);
    } else if (dy < 0) {
        movement->lookDown(-dy);
    }

    glfwSetCursorPos(window, 1100 / 2, 800 / 2);
}

int main() {
    if (!initOpenGL()) {
        return -1;
    }

    GLFWwindow* window = createWindow(1100, 800, "Voxel Game");
    if (!window) {
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera camera;
    Player player(camera);
    Movement movement(player, 3.0f);
    World world;

    glfwSetWindowUserPointer(window, &movement);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);

    glEnable(GL_DEPTH_TEST);

    Renderer renderer(camera, &world);

    unsigned int shaderProgram = createShaderProgram();
    unsigned int textureID = loadTexture("assets/atlas.png");

    glfwSetCursorPos(window, 1400 / 2, 800 / 2);

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        renderer.render(shaderProgram, window);

        if (keyStates[GLFW_KEY_W]) movement.moveForward();
        if (keyStates[GLFW_KEY_A]) movement.moveLeft();
        if (keyStates[GLFW_KEY_S]) movement.moveBackward();
        if (keyStates[GLFW_KEY_D]) movement.moveRight();
        if (keyStates[GLFW_KEY_SPACE]) movement.moveUp();
        if (keyStates[GLFW_KEY_LEFT_SHIFT] || keyStates[GLFW_KEY_RIGHT_SHIFT]) movement.moveDown();

        // std::cout << "(" << player.camera.getX() << ", " << player.camera.getY() << ", " << player.camera.getZ() << ")" << std::endl;
        // std::cout << world.getBlock((int)player.camera.getX(),(int)player.camera.getY(), (int)player.camera.getZ()).type << std::endl;
        
        // int textureIndex = 5;
        // TextureCoords coords = textureMap.calculateCoords(textureIndex);

        // std::cout << "minU: " << coords.minU << ", maxU: " << coords.maxU << std::endl;
        // std::cout << "minV: " << coords.minV << ", maxV: " << coords.maxV << std::endl;


        movement.updateVectors(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
