#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include <glm/glm.hpp>

class Player {
public:
    Player();
    Player(Camera& camera);

    glm::vec3 vel;  // Velocity of the player
    Camera& camera; // Reference to the player's camera
};

#endif // PLAYER_H
