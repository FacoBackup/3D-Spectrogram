#include "EngineContext.h"

#include "../../context/ApplicationContext.h"
#include "../../service/buffer/BufferInstance.h"
#include "../../service/camera/Camera.h"

namespace Metal {
    void EngineContext::onInitialize() {
        context.passesService.onInitialize();
    }

    void EngineContext::updateCurrentTime() {
        currentTime = Clock::now();
        std::chrono::duration<float> delta = currentTime - previousTime;
        deltaTime = delta.count();
        previousTime = currentTime;

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch());
        currentTimeMs = duration.count();

        if (start == -1) {
            start = currentTimeMs;
        }
    }

    void EngineContext::onSync() {
        updateCurrentTime();

        if (context.globalRepository.isNotFrozen() || context.spectrogramRepository.isNotFrozen()) {
            if ((currentTimeMs - lastTriggerTime) >= 1000) {
                std::cout << (currentTimeMs - lastTriggerTime) << std::endl;
                lastTriggerTime = currentTimeMs;
                context.globalRepository.freezeVersion();
                context.spectrogramRepository.freezeVersion();
                context.voxelProcessorService.process();
            }
        } else {
            lastTriggerTime = currentTimeMs;
        }

        context.cameraService.onSync();

        updateGlobalData();

        context.passesService.onSync();

        setCameraUpdated(false);
    }

    int EngineContext::getMaxX() const {
        if (context.spectrogramRepository.isShowingOriginalWave) {
            return static_cast<int>(SAMPLE_SIZE_SECONDS * ORIGINAL_WAVE_SCALE / 2.f);
        }
        return context.globalRepository.maxXAxis;
    }

    void EngineContext::updateGlobalData() {
        globalDataUBO.viewMatrix = camera.viewMatrix;
        globalDataUBO.projectionMatrix = camera.projectionMatrix;
        globalDataUBO.invProj = camera.invProjectionMatrix;
        globalDataUBO.invView = camera.invViewMatrix;
        globalDataUBO.cameraWorldPosition = camera.position;
        globalDataUBO.axisLengths.x = getMaxX();
        globalDataUBO.axisLengths.y = context.globalRepository.maxYAxis;
        globalDataUBO.axisLengths.z = context.globalRepository.maxZAxis;
        globalDataUBO.isOrtho = context.engineContext.camera.isOrthographic;
        globalDataUBO.isStaticCurve = context.globalRepository.isShowStaticCurve;
        context.coreBuffers.globalData->update(&globalDataUBO);
    }
}
