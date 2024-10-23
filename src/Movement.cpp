#include "Movement.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Movement::Movement(Player& player, World& world, float speed, bool gravity)
    : player(player), world(world), speed(speed), verticalSpeed(speed / 3.0f), gravity(gravity), gravityAcceleration(-40.0f), maxFallSpeed(-250.0f) {}

void Movement::moveForward() {
    if (gravity){
        collisionDetectAndMove(forward);
    } 
    else {
        glm::vec3 forward = glm::normalize(glm::vec3(
            sin(glm::radians(player.camera.getRotY())),
            0.0f,
            -cos(glm::radians(player.camera.getRotY()))
        ));
        player.vel += speed * forward;
    }
}

void Movement::moveRight() {
    if (gravity) {
        collisionDetectAndMove(right);
    }
    else {

    glm::vec3 right = glm::normalize(glm::vec3(
        cos(glm::radians(player.camera.getRotY())),
        0.0f,
        sin(glm::radians(player.camera.getRotY()))
    ));
    player.vel += speed * right;
    }
}

void Movement::moveBackward() {
    if (gravity) {
        collisionDetectAndMove(backward);
    }
    else {
        glm::vec3 backward = glm::normalize(glm::vec3(
            -sin(glm::radians(player.camera.getRotY())),
            0.0f,
            cos(glm::radians(player.camera.getRotY()))
        ));
        player.vel += speed * backward;
    }
}

void Movement::moveLeft() {
    if (gravity) {
        collisionDetectAndMove(left);
    }
    else {
        glm::vec3 left = glm::normalize(glm::vec3(
            -cos(glm::radians(player.camera.getRotY())),
            0.0f,
            -sin(glm::radians(player.camera.getRotY()))
        ));
        player.vel += speed * left;
    }
}

void Movement::moveUp() {
    if (gravity){
        if (isOnGround()){
        player.vel.y = 16;

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

void Movement::collisionDetectAndMove(int facing){
    bool touchingPosX, touchingPosZ, touchingNegX, touchingNegZ;
    int result = no_collision;

    // positive X direction
    if (world.getBlock(player.camera.getX() + 0.2, player.camera.getY(), player.camera.getZ()).type != AIR
        || world.getBlock(player.camera.getX() + 0.2, player.camera.getY() - 1, player.camera.getZ()).type != AIR) {
            touchingPosX = true;
    }

    // negative X direction
    if (world.getBlock(player.camera.getX() - 0.2, player.camera.getY(), player.camera.getZ()).type != AIR
        || world.getBlock(player.camera.getX() - 0.2, player.camera.getY() - 1, player.camera.getZ()).type != AIR) {
            touchingNegX = true;
    }

    // positive Z direction
    if (world.getBlock(player.camera.getX(), player.camera.getY(), player.camera.getZ() + 0.2).type != AIR
        || world.getBlock(player.camera.getX(), player.camera.getY() - 1, player.camera.getZ() + 0.2).type != AIR) {
            touchingPosZ = true;
    }

    // negative Z direction
    if (world.getBlock(player.camera.getX(), player.camera.getY(), player.camera.getZ() - 0.2).type != AIR
        || world.getBlock(player.camera.getX(), player.camera.getY() - 1, player.camera.getZ() - 0.2).type != AIR) {
            touchingNegZ = true;
    }

    int mask =  (touchingPosX ? 1 : 0) | 
                (touchingNegX ? 2 : 0) | 
                (touchingPosZ ? 4 : 0) | 
                (touchingNegZ ? 8 : 0);

    switch (mask) {
        case 5: // 0101: touchingPosX + touchingPosZ
            result = CollisionCase::posXposZ;
            break;
        case 9: // 1001: touchingPosX + touchingNegZ
            result = CollisionCase::posXnegZ;
            break;
        case 6: // 0110: touchingNegX + touchingPosZ
            result = CollisionCase::negXposZ;
            break;
        case 10: // 1010: touchingNegX + touchingNegZ
            result = CollisionCase::negXnegZ;
            break;
        case 1: // 0001: touchingPosX only
            result = CollisionCase::posX;
            break;
        case 2: // 0010: touchingNegX only
            result = CollisionCase::negX;
            break;
        case 4: // 0100: touchingPosZ only
            result = CollisionCase::posZ;
            break;
        case 8: // 1000: touchingNegZ only
            result = CollisionCase::negZ;
            break;
        default:
            result = CollisionCase::no_collision;
            break;
    }
    if (result != no_collision){

    std::cout << "Collision" << std::endl;
    }

    groundMove((result + facing) % 8, facing);
    return;
}

void Movement::groundMove(int direction, int facing) {
    float speed = 0.5f;
    glm::vec3 moveVector;

    if (direction == CollisionCase::no_collision) {
        moveVector = glm::normalize(glm::vec3(
            sin(glm::radians(player.camera.getRotY() + 90 * facing)),
            0.0f,
            -cos(glm::radians(player.camera.getRotY() + 90 * facing))
        ));
    } 
    else {
        moveVector = glm::normalize(glm::vec3(
            sin(glm::radians(player.camera.getRotY() + 45 * direction + 180)),
            0.0f,
            -cos(glm::radians(player.camera.getRotY() + 45 * direction + 180))
        ));
    }

    player.vel += speed * moveVector;
}
