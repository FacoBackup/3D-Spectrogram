#ifndef GRIDRENDERPASS_H
#define GRIDRENDERPASS_H
#include "../../AbstractRenderPass.h"

namespace Metal {
    class GridPass final : public AbstractRenderPass {
    public:
        explicit GridPass(ApplicationContext &context)
            : AbstractRenderPass(context) {
        }

        void onInitialize() override;

        void onSync() override;
    };
} // Metal

#endif //GRIDRENDERPASS_H
