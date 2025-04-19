#include "PassesService.h"
#include "../../../context/ApplicationContext.h"
#include "./CommandBufferRecorder.h"
#include "../render-pass/impl/tools/BackgroundPass.h"
#include "../render-pass/impl/tools/GridPass.h"
#include "../render-pass/impl/tools/VoxelVisualizerPass.h"

namespace Metal {
    PassesService::PassesService(ApplicationContext &context) : AbstractRuntimeComponent(context) {
    }

    void PassesService::onInitialize() {
        recorder = new CommandBufferRecorder(context.coreFrameBuffers.imageFBO, context);
        voxelPass = new VoxelVisualizerPass(context);
        gridPass = new GridPass(context);
        backgroundPass = new BackgroundPass(context);

        voxelPass->onInitialize();
        gridPass->onInitialize();
        backgroundPass->onInitialize();
    }

    void PassesService::onSync() {
        recorder->recordCommands({backgroundPass, gridPass, voxelPass});
    }
} // Metal
