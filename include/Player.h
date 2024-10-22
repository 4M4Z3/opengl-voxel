#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "Vec3.h"

class Player {
public:
    Player();
    Player(Camera &camera);
    Vec3 vel;
    Camera &camera;
};

#endif // PLAYER_H
