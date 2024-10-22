#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Player.h"

class Movement {
    public:

    Movement(Player &player);
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

    void moveCamera(float dx, float dy);

    Player &player;

};


#endif // MOVEMENT_H