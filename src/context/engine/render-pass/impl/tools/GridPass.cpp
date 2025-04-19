#include "GridPass.h"

#include "../../../../../context/ApplicationContext.h"
#include "../../../../../service/pipeline/PipelineBuilder.h"

namespace Metal {
    void GridPass::onInitialize() {
        PipelineBuilder gridPipelineBuilder = PipelineBuilder::Of(
                    context.coreFrameBuffers.imageFBO,
                    "QUAD.vert",
                    "tools/Grid.frag"
                )
                .addDescriptorSet(context.coreDescriptorSets.globalDataDescriptor.get());
        pipelineInstance = context.pipelineService.createPipeline(gridPipelineBuilder);
    }

    void GridPass::onSync() {
        recordDrawSimpleInstanced(3, 1);
    }
} // Metal
