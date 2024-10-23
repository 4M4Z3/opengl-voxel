#include "Camera.h"

Camera::Camera() : position(0.0f, 70.0f, 0.0f), rotation(0.0f), fov(75.0f) {}

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, float fov)
    : position(position), rotation(rotation), fov(fov) {}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = glm::identity<glm::mat4>();
    view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    view = glm::translate(view, -position);
    return view;
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Camera::setRotation(const glm::vec3& rotation) {
    this->rotation = rotation;
}

glm::vec3 Camera::getForward() const {
    float yaw = glm::radians(rotation.y);
    return glm::normalize(glm::vec3(
        sin(yaw),
        0.0f,
        -cos(yaw)
    ));
}

void Camera::setFOV(float fov) {
    this->fov = fov;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getRotation() const {
    return rotation;
}

float Camera::getFOV() const {
    return fov;
}

// Individual position components
void Camera::setX(float x) {
    position.x = x;
}

void Camera::setY(float y) {
    position.y = y;
}

void Camera::setZ(float z) {
    position.z = z;
}

float Camera::getX() const {
    return position.x;
}

float Camera::getY() const {
    return position.y;
}

float Camera::getZ() const {
    return position.z;
}

// Individual rotation components
void Camera::setRotX(float rotX) {
    rotation.x = rotX;
}

void Camera::setRotY(float rotY) {
    rotation.y = rotY;
}

float Camera::getRotX() const {
    return rotation.x;
}

float Camera::getRotY() const {
    return rotation.y;
}
