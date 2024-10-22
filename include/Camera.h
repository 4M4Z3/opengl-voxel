#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(const glm::vec3& position, const glm::vec3& rotation, float fov);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setFOV(float fov);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    float getFOV() const;

    // Individual position components
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    float getX() const;
    float getY() const;
    float getZ() const;

    // Individual rotation components
    void setRotX(float rotX);
    void setRotY(float rotY);
    float getRotX() const;
    float getRotY() const;

private:
    glm::vec3 position;
    glm::vec3 rotation; // Pitch (X), Yaw (Y), Roll (Z)
    float fov;
};

#endif // CAMERA_H
