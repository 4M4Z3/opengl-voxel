#include "Movement.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Movement::Movement(Player& player, float speed)
    : player(player), speed(speed), verticalSpeed(speed / 3.0f) {
}

void Movement::moveForward() {
    glm::vec3 forward = glm::normalize(glm::vec3(
        sin(glm::radians(player.camera.getRotY())),
        0.0f,
        -cos(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed * forward;
}

void Movement::moveBackward() {
    glm::vec3 backward = glm::normalize(glm::vec3(
        -sin(glm::radians(player.camera.getRotY())),
        0.0f,
        cos(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed * backward;
}


void Movement::moveRight() {
    glm::vec3 right = glm::normalize(glm::vec3(
        cos(glm::radians(player.camera.getRotY())),
        0.0f,
        sin(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed * right;
}

void Movement::moveLeft() {
    glm::vec3 left = glm::normalize(glm::vec3(
        -cos(glm::radians(player.camera.getRotY())),
        0.0f,
        -sin(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed * left;
}

void Movement::moveUp() {
    player.vel.y += verticalSpeed;
}

void Movement::moveDown() {
    player.vel.y -= verticalSpeed;
}

void Movement::lookRight(double dx) {
    player.camera.setRotY(player.camera.getRotY() + dx);
}

void Movement::lookLeft(double dx) {
    player.camera.setRotY(player.camera.getRotY() - dx);
}

void Movement::lookDown(double dy) {
    float newRotX = player.camera.getRotX() - dy;
    if (newRotX < -89.0f) {
        newRotX = -89.0f;
    }
    player.camera.setRotX(newRotX);
}

void Movement::lookUp(double dy) {
    float newRotX = player.camera.getRotX() + dy;
    if (newRotX > 89.0f) {
        newRotX = 89.0f;
    }
    player.camera.setRotX(newRotX);
}


void Movement::updateVectors(float deltaTime) {
    glm::vec3 newPosition = player.camera.getPosition() + player.vel * deltaTime;
    player.camera.setPosition(newPosition);

    float friction = 2.0f;
    player.vel -= player.vel * friction * deltaTime;

    if (glm::length(player.vel) < 0.01f) {
        player.vel = glm::vec3(0.0f);
    }
}
