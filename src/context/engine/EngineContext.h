#ifndef TCC_IESB_ENGINECONTEXT_H
#define TCC_IESB_ENGINECONTEXT_H

#include <chrono>
#include <glm/ext/scalar_constants.hpp>

#include "../../dto/ubo/GlobalDataUBO.h"
#include "../../common/AbstractRuntimeComponent.h"
#include "../../service/camera/Camera.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

namespace Metal {
    struct EngineContext final : AbstractRuntimeComponent {
        Camera camera{};

        GlobalDataUBO globalDataUBO{};
        unsigned int lightsCount = 0;
        long long start = -1;
        bool cameraUpdated = true;
        long long currentTimeMs = 0;
        TimePoint currentTime;
        TimePoint previousTime = Clock::now();
        float deltaTime = 0;

        GlobalDataUBO &getGlobalDataUBO() { return globalDataUBO; }


        void setCameraUpdated(const bool val) {
            cameraUpdated = val;
        }

        bool isCameraUpdated() const {
            return cameraUpdated;
        }

        void onInitialize() override;

        void updateVoxelData();

        void updateCurrentTime();

        explicit EngineContext(ApplicationContext &context) : AbstractRuntimeComponent(context) {
        }


        void updateGlobalData();

        void onSync() override;
    };
}
#endif
