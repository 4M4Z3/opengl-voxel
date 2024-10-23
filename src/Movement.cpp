#include "Movement.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Movement::Movement(Player& player, World& world, float speed, bool gravity)
    : player(player), world(world), speed(speed), verticalSpeed(speed / 3.0f), gravity(gravity), gravityAcceleration(-9.8f), maxFallSpeed(-50.0f) {}

void Movement::moveForward() {
    glm::vec3 forward = glm::normalize(glm::vec3(
        sin(glm::radians(player.camera.getRotY())),
        0.0f,
        -cos(glm::radians(player.camera.getRotY()))
    ));

    glm::vec3 nextPosition = player.camera.getPosition() + forward * (speed + 0.1f);
    if (!checkCollision(nextPosition) && !checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.vel += speed * forward;
    }
}

void Movement::moveBackward() {
    glm::vec3 backward = glm::normalize(glm::vec3(
        -sin(glm::radians(player.camera.getRotY())),
        0.0f,
        cos(glm::radians(player.camera.getRotY()))
    ));

    glm::vec3 nextPosition = player.camera.getPosition() + backward * (speed + 0.1f);
    if (!checkCollision(nextPosition) && !checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.vel += speed * backward;
    }
}

void Movement::moveRight() {
    glm::vec3 right = glm::normalize(glm::vec3(
        cos(glm::radians(player.camera.getRotY())),
        0.0f,
        sin(glm::radians(player.camera.getRotY()))
    ));

    glm::vec3 nextPosition = player.camera.getPosition() + right * (speed + 0.1f);
    if (!checkCollision(nextPosition) && !checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.vel += speed * right;
    }
}

void Movement::moveLeft() {
    glm::vec3 left = glm::normalize(glm::vec3(
        -cos(glm::radians(player.camera.getRotY())),
        0.0f,
        -sin(glm::radians(player.camera.getRotY()))
    ));

    glm::vec3 nextPosition = player.camera.getPosition() + left * (speed + 0.1f);
    if (!checkCollision(nextPosition) && !checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.vel += speed * left;
    }
}

void Movement::moveUp() {
    glm::vec3 nextPosition = player.camera.getPosition() + glm::vec3(0.0f, verticalSpeed + 0.1f, 0.0f);
    if (!checkCollision(nextPosition) && !checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.vel.y += verticalSpeed;
    }
}

void Movement::moveDown() {
    glm::vec3 nextPosition = player.camera.getPosition() + glm::vec3(0.0f, -verticalSpeed - 0.1f, 0.0f);
    if (!checkCollision(nextPosition) && !checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.vel.y -= verticalSpeed;
    }
}

bool Movement::isCollidingWithBlock(const glm::vec3& direction) {
    glm::vec3 nextPosition = player.camera.getPosition() + direction * (speed + 0.1f);
    return checkCollision(nextPosition) || checkCollision(nextPosition - glm::vec3(0.0f, 1.0f, 0.0f)) || checkCollision(nextPosition - glm::vec3(0.0f, 2.0f, 0.0f));
}

bool Movement::checkCollision(const glm::vec3& nextPosition) {
    return !isAirBlock(nextPosition);
}

bool Movement::isAirBlock(const glm::vec3& position) {
    return world.getBlock(position.x, position.y, position.z).type == AIR;
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
        player.vel.y += gravityAcceleration * deltaTime;

        if (player.vel.y < maxFallSpeed) {
            player.vel.y = maxFallSpeed;
        }

        bool isOnGround = this->isOnGround();
        if (isOnGround && player.vel.y < 0) {
            player.vel.y = 0.0f;
        }
    }

    glm::vec3 newPosition = player.camera.getPosition() + player.vel * deltaTime;
    if (!checkCollision(newPosition) && !checkCollision(newPosition - glm::vec3(0.0f, 1.0f, 0.0f)) && !checkCollision(newPosition - glm::vec3(0.0f, 2.0f, 0.0f))) {
        player.camera.setPosition(newPosition);
    } else {
        player.vel = glm::vec3(0.0f); 
    }

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
    return world.getBlock(player.camera.getX(), player.camera.getY() - 2, player.camera.getZ()).type != AIR;
}
