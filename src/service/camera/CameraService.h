#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "../../common/AbstractRuntimeComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float PI_OVER_2 = glm::pi<float>() / 2.0f;
constexpr float PI_2 = glm::pi<float>() * 2.0f;

namespace Metal {
    class ApplicationContext;
    struct Camera;

    class CameraService final : public AbstractRuntimeComponent {
        Camera *camera = nullptr;
        glm::vec3 xAxis{0.0f};
        glm::vec3 yAxis{0.0f};
        glm::vec3 zAxis{0.0f};

        void updateMatrices();

        void updateView();

        void updateProjection() const;

        void updateAspectRatio() const;

        void createViewMatrix();

    public:
        void handleInput(bool isFirstMovement) const;

        void handleScroll(float scrollDelta) const;

        void updateCameraTarget();

        explicit CameraService(ApplicationContext &context);

        void onSync() override;
    };
}

#endif
