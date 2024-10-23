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
#include <glm/gtc/type_ptr.hpp>

void setFogParameters(unsigned int shaderProgram, const Camera& camera, const World& world) {
    // Fog parameters
    glm::vec3 fogColor(0.5f, 0.7f, 1.0f);
    glm::vec3 underwaterFogColor(0.0f, 0.0f, 0.4f);
    float fogStart = 100.0f; 
    float fogEnd = 120.0f;  

    glm::vec3 cameraPos = camera.getPosition();
    int blockX = static_cast<int>(cameraPos.x);
    int blockY = static_cast<int>(cameraPos.y);
    int blockZ = static_cast<int>(cameraPos.z);

    BlockType blockAtCamera = world.getBlock(blockX, blockY, blockZ).type;
    bool isUnderwater = (blockAtCamera == WATER);

    glm::vec3 activeFogColor = isUnderwater ? underwaterFogColor : fogColor;

    int fogColorLoc = glGetUniformLocation(shaderProgram, "fogColor");
    glUniform3fv(fogColorLoc, 1, glm::value_ptr(activeFogColor));

    int fogStartLoc = glGetUniformLocation(shaderProgram, "fogStart");
    glUniform1f(fogStartLoc, fogStart);

    int fogEndLoc = glGetUniformLocation(shaderProgram, "fogEnd");
    glUniform1f(fogEndLoc, fogEnd);

    int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));
}

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

        // Use nearest-neighbor filtering for sharp textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
    glEnable(GL_CULL_FACE); // Enable face culling
    glCullFace(GL_BACK);    // Cull back-facing triangles
    glFrontFace(GL_CCW);    // Counter-clockwise defines the front face

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
// Vertex Shader
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aAmbientOcclusion;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out float AmbientOcclusion;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    AmbientOcclusion = aAmbientOcclusion;
}
)";

    const char* fragmentShaderSource = R"(
// Fragment Shader
#version 330 core
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in float AmbientOcclusion;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientLight;

void main() {
    // Sample the texture color
    vec4 textureColor = texture(ourTexture, TexCoord);

    // Ambient Occlusion effect: use the AO value to darken the color at corners
    float aoEffect = clamp(AmbientOcclusion, 0.0, 1.0);

    // Calculate diffuse lighting based on light direction and surface normal
    vec3 norm = normalize(Normal);
    float diffuse = max(dot(norm, -lightDir), 0.0);

    // Calculate the final color using ambient lighting, diffuse, and AO effect
    vec3 finalColor = (ambientLight + lightColor * diffuse) * textureColor.rgb * aoEffect;

    // Output the final fragment color
    FragColor = vec4(finalColor, textureColor.a);
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

    if (dx != 0) {
        movement->lookRight(dx);
    }
    if (dy != 0) {
        movement->lookUp(dy);
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
    World world;
    Movement movement(player, world, 3.0f, false);

    glfwSetWindowUserPointer(window, &movement);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);

    Renderer renderer(camera, &world);
    unsigned int shaderProgram = createShaderProgram();
    unsigned int textureID = loadTexture("assets/atlas.png");

    glfwSetCursorPos(window, 1100 / 2, 800 / 2);

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

        // Set lighting parameters before rendering
        glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, -1.0f, 0.5f));
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 ambientLight = glm::vec3(0.3f, 0.3f, 0.3f);

        glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "ambientLight"), 1, glm::value_ptr(ambientLight));

        // Set fog parameters
        setFogParameters(shaderProgram, camera, world);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(1100) / 800);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        renderer.render(shaderProgram, window);

        if (keyStates[GLFW_KEY_W]) movement.moveForward();
        if (keyStates[GLFW_KEY_A]) movement.moveLeft();
        if (keyStates[GLFW_KEY_S]) movement.moveBackward();
        if (keyStates[GLFW_KEY_D]) movement.moveRight();
        if (keyStates[GLFW_KEY_SPACE]) movement.moveUp();
        if (keyStates[GLFW_KEY_LEFT_SHIFT] || keyStates[GLFW_KEY_RIGHT_SHIFT]) movement.moveDown();

        movement.updateVectors(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
