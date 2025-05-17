#ifndef CAMERA_H
#define CAMERA_H
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../definitions.h"

constexpr float MIN_MAX_PITCH = glm::radians(89.0f);

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

        float pitch = 0.523599;
        float yaw = 0.785398;

        float lastMouseX = 0;
        float lastMouseY = 0;
        float deltaX = 0;
        float deltaY = 0;
        bool changed = false;

        bool isOrthographic = true;
        float orthographicProjectionSize = 50;

        glm::vec3 target = glm::vec3(SAMPLE_SIZE_SECONDS / 2.f);
        float orbitDistance = 25.0f;
    };
}

#endif
