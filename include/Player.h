#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include <glm/glm.hpp>

class Player {
public:
    Player();
    Player(Camera& camera);

    glm::vec3 vel; 
    Camera& camera;
};

#endif // PLAYER_H
