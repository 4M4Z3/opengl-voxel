#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Player.h"
#include "World.h"
#include <glm/glm.hpp>

class Movement {
public:
    Movement(Player& player, World& world, float speed, bool gravity);

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    void lookRight(double dx);
    void lookLeft(double dx);
    void lookDown(double dy);
    void lookUp(double dy);

    void updateVectors(float deltaTime);
    bool isOnGround();

private:
    Player& player;
    float speed; 
    float verticalSpeed;
    World world;
    bool gravity;
    float gravityAcceleration = -9.8f;
    float maxFallSpeed = -50.0f;
    bool isCollidingWithBlock(const glm::vec3& direction);
    bool checkCollision(const glm::vec3& nextPosition);
    bool isAirBlock(const glm::vec3& position);
};

#endif // MOVEMENT_H
