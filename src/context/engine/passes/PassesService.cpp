#include "PassesService.h"
#include "../../../context/ApplicationContext.h"
#include "./CommandBufferRecorder.h"
#include "../render-pass/impl/tools/GridPass.h"
#include "../render-pass/impl/tools/VoxelVisualizerPass.h"

namespace Metal {
    PassesService::PassesService(ApplicationContext &context) : AbstractRuntimeComponent(context) {
    }

    void PassesService::onInitialize() {
        recorder = new CommandBufferRecorder(context.coreFrameBuffers.postProcessingFBO, context);
        pass = new VoxelVisualizerPass(context);
        gridPass = new GridPass(context);

        pass->onInitialize();
        gridPass->onInitialize();
    }

    void PassesService::onSync() {
        recorder->recordCommands({gridPass, pass});
    }
} // Metal
