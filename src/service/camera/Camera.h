#ifndef CAMERA_H
#define CAMERA_H
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../../common/inspection/Inspectable.h"
#include "../../util/serialization-definitions.h"

namespace Metal {
    struct Camera final {
        float rotationSensitivity = 1;
        float movementSensitivity = 1.0f;
        float zoomSensitivity = 1.0f;

        glm::mat4x4 viewMatrix{};
        glm::mat4x4 projectionMatrix{};
        glm::mat4x4 invViewMatrix{};
        glm::mat4x4 invProjectionMatrix{};
        glm::mat4x4 projViewMatrix{};

        glm::vec3 position{};

        float zFar = 10000;
        float zNear = .1f;
        float fov = 90;
        float aspectRatio = 1;

        float pitch = -10.f;
        float yaw = 0.f;

        float lastMouseX = 0;
        float lastMouseY = 0;
        float deltaX = 0;
        float deltaY = 0;
        bool changed = false;

        glm::vec3 target = glm::vec3(0.0f); // Center to orbit around
        float orbitDistance = 25.0f;         // Radius of the orbit

        explicit Camera(const float pitch, const float yaw, const glm::vec3 position) {
            this->pitch = pitch;
            this->yaw = yaw;
            this->position = position;
        }
    };
}

#endif
