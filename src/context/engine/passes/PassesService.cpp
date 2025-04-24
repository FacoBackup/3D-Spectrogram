#include "PassesService.h"
#include "../../../context/ApplicationContext.h"
#include "./CommandBufferRecorder.h"
#include "../render-pass/impl/tools/VisualizerPass.h"

namespace Metal {
    PassesService::PassesService(ApplicationContext &context) : AbstractRuntimeComponent(context) {
    }

    void PassesService::onInitialize() {
        recorder = new CommandBufferRecorder(context.coreFrameBuffers.imageFBO, context);
        voxelPass = new VisualizerPass(context);
        voxelPass->onInitialize();
    }

    void PassesService::onSync() {
        recorder->recordCommands({voxelPass});
    }
} // Metal
