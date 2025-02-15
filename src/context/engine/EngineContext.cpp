#include "EngineContext.h"

#include "../../context/ApplicationContext.h"
#include "../../service/buffer/BufferInstance.h"
#include "../../service/voxel/SVOInstance.h"
#include "../../enum/LevelOfDetail.h"
#include "../../enum/LightType.h"
#include "../../service/camera/Camera.h"
#include "../../service/framebuffer/FrameBufferInstance.h"
#include "../../service/texture/TextureInstance.h"

namespace Metal {
    void EngineContext::onInitialize() {
        context.passesService.onInitialize();
    }

    void EngineContext::updateVoxelData() {
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

    void EngineContext::dispatchSceneVoxelization() {
        voxelizationRequestId = Util::uuidV4();
    }

    void EngineContext::onSync() {
        updateCurrentTime();

        context.cameraService.onSync();

        updateGlobalData();
        updateVoxelData();

        context.passesService.onSync();

        setLightingDataUpdated(false);
        setCameraUpdated(false);
        setGISettingsUpdated(false);
    }

    void EngineContext::updateGlobalData() {
        globalDataUBO.viewMatrix = camera.viewMatrix;
        globalDataUBO.projectionMatrix = camera.projectionMatrix;
        globalDataUBO.projView = camera.projViewMatrix;
        globalDataUBO.invProj = camera.invProjectionMatrix;
        globalDataUBO.invView = camera.invViewMatrix;
        globalDataUBO.cameraWorldPosition = camera.position;
        globalDataUBO.giStrength = context.engineRepository.giStrength;
        globalDataUBO.lightCount = lightsCount;
        globalDataUBO.isAtmosphereEnabled = context.engineRepository.atmosphereEnabled;

        globalDataUBO.enabledDenoiser = context.engineRepository.enabledDenoiser;
        globalDataUBO.multipleImportanceSampling = context.engineRepository.multipleImportanceSampling;
        globalDataUBO.giMaxAccumulation = context.engineRepository.giMaxAccumulation;
        globalDataUBO.giSamples = context.engineRepository.giSamples;
        globalDataUBO.giBounces = context.engineRepository.giBounces;
        globalDataUBO.giTileSubdivision = context.engineRepository.giTileSubdivision;
        globalDataUBO.giEmissiveFactor = context.engineRepository.giEmissiveFactor;
        globalDataUBO.denoiserNoiseThreshold = context.engineRepository.denoiserNoiseThreshold;

        globalDataUBO.surfaceCacheWidth = SURFACE_CACHE_RES;
        globalDataUBO.surfaceCacheHeight = SURFACE_CACHE_RES;
        globalDataUBO.giAccumulationCount++;
        globalDataUBO.globalFrameCount++;

        if (context.engineRepository.incrementTime) {
            context.engineRepository.elapsedTime += .0005f * context.engineRepository.elapsedTimeSpeed;
            setGISettingsUpdated(true);
            lightingDataUpdated = true;
        }
        globalDataUBO.sunPosition = glm::vec3(0,
                                              std::cos(context.engineRepository.elapsedTime),
                                              std::sin(context.engineRepository.elapsedTime)) * context.engineRepository
                                    .sunDistance;
        context.coreBuffers.globalData->update(&globalDataUBO);
    }

}
