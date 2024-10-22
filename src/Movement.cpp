#include "Movement.h"
#include <cmath> 

Movement::Movement(Player& player, float speed)
    : player(player), speed(speed), verticalSpeed(speed / 3.0f) {
}

void Movement::moveForward() {
    std::cout << "Move forward" << std::endl;
    float yawRadians = player.camera.rot.y * M_PI / 180.0f;
    float pitchRadians = player.camera.rot.x * M_PI / 180.0f;

    player.vel.z += speed * cos(yawRadians) * cos(pitchRadians);
    player.vel.x += -speed * sin(yawRadians) * cos(pitchRadians);
}

void Movement::moveBackward() {
    std::cout << "Move backward" << std::endl;
    float yawRadians = player.camera.rot.y * M_PI / 180.0f;
    float pitchRadians = player.camera.rot.x * M_PI / 180.0f;

    player.vel.z -= speed * cos(yawRadians) * cos(pitchRadians);
    player.vel.x -= -speed * sin(yawRadians) * cos(pitchRadians);
}

void Movement::moveRight() {
    std::cout << "Move right" << std::endl;
    float yawRadians = player.camera.rot.y * M_PI / 180.0f;

    player.vel.z += speed * sin(yawRadians);
    player.vel.x += speed * cos(yawRadians);
}

void Movement::moveLeft() {
    std::cout << "Move left" << std::endl;
    float yawRadians = player.camera.rot.y * M_PI / 180.0f;

    player.vel.z -= speed * sin(yawRadians);
    player.vel.x -= speed * cos(yawRadians);
}

void Movement::moveUp() {
    std::cout << "Move up" << std::endl;
    player.camera.pos.y += verticalSpeed; 
}

void Movement::moveDown() {
    std::cout << "Move down" << std::endl;
    player.camera.pos.y -= verticalSpeed;
}

void Movement::lookRight(double dx) {
    player.camera.rot.y -= dx;
}

void Movement::lookLeft(double dx) {
    player.camera.rot.y += dx;
}

void Movement::lookDown(double dy) {
    player.camera.rot.x += dy;

    if (player.camera.rot.x > 89.0) {
        player.camera.rot.x = 89.0;
    }
}

void Movement::lookUp(double dy) {
    player.camera.rot.x -= dy;

    if (player.camera.rot.x < -89.0) {
        player.camera.rot.x = -89.0;
    }
}

void Movement::updateVectors(float deltaTime) {
    player.camera.pos.x += player.vel.x * deltaTime;
    player.camera.pos.y += player.vel.y * deltaTime;
    player.camera.pos.z += player.vel.z * deltaTime;

    float friction = 2.0f;
    player.vel.x -= player.vel.x * friction * deltaTime;
    player.vel.y -= player.vel.y * friction * deltaTime;
    player.vel.z -= player.vel.z * friction * deltaTime;

    if (std::abs(player.vel.x) < 0.01f) player.vel.x = 0;
    if (std::abs(player.vel.y) < 0.01f) player.vel.y = 0;
    if (std::abs(player.vel.z) < 0.01f) player.vel.z = 0;
}
