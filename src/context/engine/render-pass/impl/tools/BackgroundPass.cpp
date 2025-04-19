#include "BackgroundPass.h"

#include "../../../../../context/ApplicationContext.h"
#include "../../../../../service/pipeline/PipelineBuilder.h"

namespace Metal {
    void BackgroundPass::onInitialize() {
        PipelineBuilder pipelineBuilder = PipelineBuilder::Of(
                    context.coreFrameBuffers.imageFBO,
                    "QUAD.vert",
                    "tools/Background.frag"
                )
                .setPushConstantsSize(sizeof(BackgroundPushConstant));
        pipelineInstance = context.pipelineService.createPipeline(pipelineBuilder);
    }

    void BackgroundPass::onSync() {
        pushConstant.color = context.editorRepository.backgroundColor;
        recordPushConstant(&pushConstant);
        recordDrawSimpleInstanced(3, 1);
    }
} // Metal
