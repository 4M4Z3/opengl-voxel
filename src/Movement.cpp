#include "Movement.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Movement::Movement(Player& player, World& world, float speed, bool gravity)
    : player(player), world(world), speed(speed), verticalSpeed(speed / 5.0f), gravity(gravity), gravityAcceleration(-9.8f), maxFallSpeed(-50.0f) {}

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
    if (gravity) {
        // Apply gravity force
        player.vel.y += gravityAcceleration * deltaTime;

        // Clamp the fall speed to a maximum terminal velocity
        if (player.vel.y < maxFallSpeed) {
            player.vel.y = maxFallSpeed;
        }

        // Check for ground collision
        bool isOnGround = this->isOnGround();
        if (isOnGround && player.vel.y < 0) {
            player.vel.y = 0.0f;
        }
    }

    // Update position based on velocity
    glm::vec3 newPosition = player.camera.getPosition() + player.vel * deltaTime;
    player.camera.setPosition(newPosition);

    // Apply friction to horizontal movement when on the ground
    if (isOnGround()) {
        float friction = 2.0f;
        player.vel.x -= player.vel.x * friction * deltaTime;
        player.vel.z -= player.vel.z * friction * deltaTime;
    }

    // Stop movement if velocity is very small
    if (glm::length(glm::vec3(player.vel.x, 0.0f, player.vel.z)) < 0.01f) {
        player.vel.x = 0.0f;
        player.vel.z = 0.0f;
    }
}

bool Movement::isOnGround() {
    glm::vec3 position = player.camera.getPosition();
    return world.getBlock(player.camera.getX(), player.camera.getY()-2, player.camera.getZ()).type != AIR;
}
