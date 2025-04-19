#ifndef BACKGROUNDPASS_H
#define BACKGROUNDPASS_H
#include "../../AbstractRenderPass.h"
#include "../../../../../dto/push-constant/BackgroundPushConstant.h"

namespace Metal {
    class BackgroundPass final : public AbstractRenderPass {
        BackgroundPushConstant pushConstant{};

    public:
        explicit BackgroundPass(ApplicationContext &context)
            : AbstractRenderPass(context) {
        }

        void onInitialize() override;

        void onSync() override;
    };
} // Metal

#endif // BACKGROUNDPASS_H
