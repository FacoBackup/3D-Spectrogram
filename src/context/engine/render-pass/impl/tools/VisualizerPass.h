#ifndef VOXELVISUALIZERPASS_H
#define VOXELVISUALIZERPASS_H

#include "../../AbstractRenderPass.h"
#include "../../../../../dto/push-constant/VoxelDebugSettingsPushConstant.h"

namespace Metal {
    class VisualizerPass final : public AbstractRenderPass {
        VoxelDebugSettingsPushConstant settings{};

    public:
        explicit VisualizerPass(ApplicationContext &context)
            : AbstractRenderPass(context) {
        }

        void onInitialize() override;

        void onSync() override;
    };
} // Metal

#endif //VOXELVISUALIZERPASS_H
