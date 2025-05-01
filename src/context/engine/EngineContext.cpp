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

        context.cameraService.onSync();

        updateGlobalData();

        context.passesService.onSync();

        setCameraUpdated(false);
    }

    void EngineContext::updateGlobalData() {
        globalDataUBO.viewMatrix = camera.viewMatrix;
        globalDataUBO.projectionMatrix = camera.projectionMatrix;
        globalDataUBO.projView = camera.projViewMatrix;
        globalDataUBO.invProj = camera.invProjectionMatrix;
        globalDataUBO.invView = camera.invViewMatrix;
        globalDataUBO.cameraWorldPosition = camera.position;
        globalDataUBO.xAxisLength = context.editorRepository.sampleSize;
        globalDataUBO.zAxisLength = context.editorRepository.maxFrequency;
        globalDataUBO.yAxisLength = context.editorRepository.maxMagnitude;
        context.coreBuffers.globalData->update(&globalDataUBO);
    }
}
