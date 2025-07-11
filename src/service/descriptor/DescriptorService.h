#ifndef DESCRIPTORPOOLSERVICE_H
#define DESCRIPTORPOOLSERVICE_H

#include "../abstract/AbstractResourceService.h"

namespace Metal {
    struct FrameBufferAttachment;
    struct FrameBufferInstance;
    struct DescriptorInstance;

    class DescriptorService final : public AbstractRuntimeComponent {
    public:
        explicit DescriptorService(ApplicationContext &context);

        void setImageDescriptor(const FrameBufferInstance *framebuffer, unsigned int attachmentIndex) const;
    };
} // Metal

#endif //DESCRIPTORPOOLSERVICE_H
