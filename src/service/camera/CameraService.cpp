#include "CameraService.h"

#include "Camera.h"
#include "../../context/ApplicationContext.h"

namespace Metal {
    void CameraService::onSync() {
        camera = &context.engineContext.camera;
        if (camera != nullptr) {
            updateAspectRatio();
            if (camera->changed) {
                updateMatrices();
                context.engineContext.setCameraUpdated(true);
                camera->changed = false;
            }
        }
    }

    void CameraService::updateAspectRatio() const {
        const auto &runtimeRepository = context.runtimeRepository;

        const float prevAspect = camera->aspectRatio;
        camera->aspectRatio = runtimeRepository.viewportW / runtimeRepository.viewportH;
        if (prevAspect != camera->aspectRatio) {
            camera->changed = true;
        }
    }

    void CameraService::updateMatrices() {
        updateProjection();
        updateView();
        camera->projViewMatrix = camera->projectionMatrix * camera->viewMatrix;
    }

    void CameraService::updateView() {
        createViewMatrix();
        camera->invViewMatrix = inverse(camera->viewMatrix);
    }

    void CameraService::updateProjection() const {
        camera->projectionMatrix = glm::perspective(camera->fov * Util::TO_RADIANS, camera->aspectRatio,
                                                    camera->zNear,
                                                    camera->zFar);
        camera->projectionMatrix[1][1] *= -1;
        camera->invProjectionMatrix = glm::inverse(camera->projectionMatrix);
    }

    CameraService::CameraService(ApplicationContext &context) : AbstractRuntimeComponent(context) {
    }

    void CameraService::handleInput(const bool isFirstMovement) const {
        const auto &runtimeRepository = context.runtimeRepository;
        const float mouseX = runtimeRepository.mouseX;
        const float mouseY = runtimeRepository.mouseY;

        if (isFirstMovement) {
            camera->lastMouseX = mouseX;
            camera->lastMouseY = mouseY;
        }
        camera->deltaX = (mouseX - camera->lastMouseX) * camera->rotationSensitivity *
                         0.25f;
        camera->deltaY = (camera->lastMouseY - mouseY) * camera->rotationSensitivity *
                         0.25f;
        camera->lastMouseX = mouseX;
        camera->lastMouseY = mouseY;

        camera->yaw -= glm::radians(camera->deltaX);
        camera->pitch -= glm::radians(camera->deltaY);
        camera->pitch = glm::clamp(camera->pitch, -MIN_MAX_PITCH, MIN_MAX_PITCH);

        camera->changed = true;
    }

    void CameraService::handleScroll(float scrollDelta) const {
        camera->orbitDistance -= scrollDelta * camera->movementSensitivity;
        camera->orbitDistance = glm::clamp(camera->orbitDistance, 0.5f, 100.0f);
        camera->changed = true;
    }

    void CameraService::createViewMatrix() {
        // Convert yaw and pitch to spherical coordinates
        float x = camera->orbitDistance * std::cos(camera->pitch) * std::sin(camera->yaw);
        float y = camera->orbitDistance * std::sin(camera->pitch);
        float z = camera->orbitDistance * std::cos(camera->pitch) * std::cos(camera->yaw);

        camera->position = camera->target + glm::vec3(x, y, z);
        camera->viewMatrix = glm::lookAt(camera->position, camera->target, glm::vec3(0, 1, 0));
    }
}
