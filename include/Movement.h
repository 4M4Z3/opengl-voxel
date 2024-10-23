#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Player.h"
#include "World.h"
#include <glm/glm.hpp>

class Movement {
public:
    enum CollisionCase {
        no_collision = -1,
        posX,
        posZ,
        negX,
        negZ,
        posXposZ,
        posXnegZ,
        negXposZ,
        negXnegZ
    };

    enum Direction {
        forward = 0,
        right,
        backward,
        left,
    };

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
    void collisionDetectAndMove(int facing);
    void groundMove(int direction, int facing);

private:
    Player& player;
    float speed; 
    float verticalSpeed;
    World world;
    bool gravity;
    float gravityAcceleration = -9.8f;
    float maxFallSpeed = -50.0f;
};

#endif // MOVEMENT_H
