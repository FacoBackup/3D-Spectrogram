#include "VisualizerPass.h"
#include "../../../../ApplicationContext.h"

#include "../../../../../service/pipeline/PipelineBuilder.h"

namespace Metal {
    void VisualizerPass::onInitialize() {
        PipelineBuilder voxelVisualizerPipelineBuilder = PipelineBuilder::Of(
                    context.coreFrameBuffers.imageFBO,
                    "QUAD.vert",
                    "RENDERER.frag"
                )
                .setPushConstantsSize(sizeof(VoxelDebugSettingsPushConstant))
                .addDescriptorSet(context.coreDescriptorSets.globalDataDescriptor.get())
                .addDescriptorSet(context.coreDescriptorSets.svoData.get());
        pipelineInstance = context.pipelineService.createPipeline(voxelVisualizerPipelineBuilder);
    }

    void VisualizerPass::onSync() {
        settings.searchCountDivisor = context.editorRepository.voxelSearchCount;
        settings.showRaySearchCount = context.editorRepository.showRaySearchCountVoxels;
        settings.showRayTestCount = context.editorRepository.showRayTestCountVoxels;
        recordPushConstant(&settings);
        recordDrawSimpleInstanced(3, 1);
    }
} // Metal
