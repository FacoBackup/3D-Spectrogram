#ifndef COREREFRAMEBUFFERS_H
#define COREREFRAMEBUFFERS_H
#include "../abstract/AbstractCoreRepository.h"


namespace Metal {
    struct FrameBufferInstance;

    struct CoreFrameBuffers final : AbstractCoreRepository {
        FrameBufferInstance *imageFBO = nullptr;

        explicit CoreFrameBuffers(ApplicationContext &context)
            : AbstractCoreRepository(context) {
        }

        void onInitialize() override;
    };
}

#endif
