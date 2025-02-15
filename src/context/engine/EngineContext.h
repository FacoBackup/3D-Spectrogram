#ifndef TCC_IESB_ENGINECONTEXT_H
#define TCC_IESB_ENGINECONTEXT_H

#include <chrono>
#include <glm/ext/scalar_constants.hpp>

#include "../../dto/ubo/GlobalDataUBO.h"
#include "../../common/AbstractRuntimeComponent.h"
#include "../../dto/ubo/LightData.h"
#include "../../dto/ubo/TileInfoUBO.h"
#include "../../service/camera/Camera.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

namespace Metal {
    struct EngineContext final : AbstractRuntimeComponent {
        Camera camera{-(glm::pi<float>() / 4), glm::pi<float>() / 4, {10, 10, 10}};

        GlobalDataUBO globalDataUBO{};
        TileInfoUBO tileInfoUBO{};
        std::vector<LightData> lights{};
        unsigned int lightsCount = 0;
        long long start = -1;
        bool cameraUpdated = true;
        bool lightingDataUpdated = true;
        bool giSettingsUpdated = true;
        std::string voxelizationRequestId;
        unsigned int giAccumulationCount = 0;
        long long currentTimeMs = 0;
        TimePoint currentTime;
        TimePoint previousTime = Clock::now();
        float deltaTime = 0;

        GlobalDataUBO &getGlobalDataUBO() { return globalDataUBO; }

        void setLightingDataUpdated(const bool val) {
            lightingDataUpdated = val;
        }

        bool isLightingDataUpdated() const {
            return lightingDataUpdated;
        }

        void setCameraUpdated(const bool val) {
            cameraUpdated = val;
        }

        bool isCameraUpdated() const {
            return cameraUpdated;
        }

        void setGISettingsUpdated(const bool val) {
            giSettingsUpdated = val;
        }

        void resetGiAccumulationCount() {
            globalDataUBO.giAccumulationCount = 0;
        }

        bool isGISettingsUpdated() const {
            return giSettingsUpdated;
        }

        void onInitialize() override;

        void updateVoxelData();

        void updateCurrentTime();

        void dispatchSceneVoxelization();

        std::string getVoxelizationRequestId() {
            return voxelizationRequestId;
        }

        explicit EngineContext(ApplicationContext &context) : AbstractRuntimeComponent(context) {
        }


        void updateGlobalData();

        void onSync() override;
    };
}
#endif
