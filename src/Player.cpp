#include "Player.h"

Player::Player() 
    : camera(*new Camera()), vel(0.0f, 0.0f, 0.0f) {
}

Player::Player(Camera& camera) 
    : camera(camera), vel(0.0f, 0.0f, 0.0f) {
}
