#include "Movement.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Movement::Movement(Player& player, World& world, float speed, bool gravity)
    : player(player), world(world), speed(speed), verticalSpeed(speed / 3.0f), gravity(gravity), gravityAcceleration(-40.0f), maxFallSpeed(-250.0f) {}

void Movement::moveForward() {
    glm::vec3 forward = glm::normalize(glm::vec3(
        sin(glm::radians(player.camera.getRotY())),
        0.0f,
        -cos(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed / 4 * forward;
    detectCollision();
}

void Movement::moveBackward() {
    glm::vec3 backward = glm::normalize(glm::vec3(
        -sin(glm::radians(player.camera.getRotY())),
        0.0f,
        cos(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed / 4 * backward;
    detectCollision();
}

void Movement::moveRight() {
    glm::vec3 right = glm::normalize(glm::vec3(
        cos(glm::radians(player.camera.getRotY())),
        0.0f,
        sin(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed / 4 * right;
    detectCollision();
}

void Movement::moveLeft() {
    glm::vec3 left = glm::normalize(glm::vec3(
        -cos(glm::radians(player.camera.getRotY())),
        0.0f,
        -sin(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed / 4 * left;
    detectCollision();
}

void Movement::moveUp() {
    if (gravity){
        if (isOnGround()){
        player.vel.y = 20;

        }
    }
    else {
        player.vel.y += verticalSpeed;
    }

}

void Movement::moveDown() {
    if (gravity){
       // do nothing, cant move down because we're not flying 
    }
    else {
        player.vel.y -= verticalSpeed;
    }
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
    } else if (newRotX < -89.0f) {
        newRotX = -89.0f;
    }
    player.camera.setRotX(newRotX);
}


void Movement::updateVectors(float deltaTime) {
    if (gravity) {
        detectCollision();
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
    player.camera.setPosition(newPosition);
    player.vel *= 0.98;
    player.vel.y *= 0.98;

    if (isOnGround()) {
        float friction = 2.0f;
        player.vel.x -= player.vel.x * friction * deltaTime;
        player.vel.z -= player.vel.z * friction * deltaTime;
    }

    if (glm::length(glm::vec3(player.vel.x, 0.0f, player.vel.z)) < 0.01f) {
        player.vel.x = 0.0f;
        player.vel.z = 0.0f;
    }
}

bool Movement::isOnGround() {
    glm::vec3 position = player.camera.getPosition();
    return world.getBlock(player.camera.getX(), player.camera.getY()-2, player.camera.getZ()).type != AIR;
}

void Movement::detectCollision() {
    float detectionDistance = 0.3f;

    // Positive X direction
    if (world.getBlock(player.camera.getX() + detectionDistance, player.camera.getY(), player.camera.getZ()).type != AIR ||
        world.getBlock(player.camera.getX() + detectionDistance, player.camera.getY() - 1, player.camera.getZ()).type != AIR) {
        if (player.vel.x > 0) {
            player.vel.x = 0;
        }
    }

    // Negative X direction
    if (world.getBlock(player.camera.getX() - detectionDistance, player.camera.getY(), player.camera.getZ()).type != AIR ||
        world.getBlock(player.camera.getX() - detectionDistance, player.camera.getY() - 1, player.camera.getZ()).type != AIR) {
        if (player.vel.x < 0) {
            player.vel.x = 0;
        }
    }

    // Positive Z direction
    if (world.getBlock(player.camera.getX(), player.camera.getY(), player.camera.getZ() + detectionDistance).type != AIR ||
        world.getBlock(player.camera.getX(), player.camera.getY() - 1, player.camera.getZ() + detectionDistance).type != AIR) {
        if (player.vel.z > 0) {
            player.vel.z = 0;
        }
    }

    // Negative Z direction
    if (world.getBlock(player.camera.getX(), player.camera.getY(), player.camera.getZ() - detectionDistance).type != AIR ||
        world.getBlock(player.camera.getX(), player.camera.getY() - 1, player.camera.getZ() - detectionDistance).type != AIR) {
        if (player.vel.z < 0) {
            player.vel.z = 0;
        }
    }
}
