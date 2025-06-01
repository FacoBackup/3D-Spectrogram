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

        if (context.editorRepository.isNotFrozen()) {
            if ((currentTimeMs - lastTriggerTime) >= 1000) {
                std::cout << (currentTimeMs - lastTriggerTime) << std::endl;
                lastTriggerTime = currentTimeMs;
                context.editorRepository.freezeVersion();
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
        if (context.editorRepository.isShowingOriginalWave) {
            return static_cast<int>(SAMPLE_SIZE_SECONDS * ORIGINAL_WAVE_SCALE / 2.f);
        }
        return context.editorRepository.maxXAxis;
    }

    void EngineContext::updateGlobalData() {
        globalDataUBO.viewMatrix = camera.viewMatrix;
        globalDataUBO.projectionMatrix = camera.projectionMatrix;
        globalDataUBO.invProj = camera.invProjectionMatrix;
        globalDataUBO.invView = camera.invViewMatrix;
        globalDataUBO.cameraWorldPosition = camera.position;
        globalDataUBO.axisLengths.x = getMaxX();
        globalDataUBO.axisLengths.y = context.editorRepository.maxYAxis;
        globalDataUBO.axisLengths.z = context.editorRepository.maxZAxis;
        globalDataUBO.isOrtho = context.engineContext.camera.isOrthographic;
        globalDataUBO.isStaticCurve = context.editorRepository.isShowStaticCurve;
        context.coreBuffers.globalData->update(&globalDataUBO);
    }
}
