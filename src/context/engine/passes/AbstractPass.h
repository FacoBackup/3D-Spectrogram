#ifndef ABSTRACTPASS_H
#define ABSTRACTPASS_H
#include <vector>
#include <vulkan/vulkan.h>
#include "../../../common/AbstractRuntimeComponent.h"

namespace Metal {
    struct MeshInstance;
    struct CoreFrameBuffers;
    struct PipelineInstance;
    struct CoreBuffers;

    class AbstractPass : public AbstractRuntimeComponent {
    public:
        VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
        PipelineInstance *pipelineInstance = nullptr;

        explicit AbstractPass(ApplicationContext &context, bool isComputePass);

        void recordPushConstant(const void *data);

        virtual bool shouldRun() {
            return true;
        }

        void setCommandBuffer(VkCommandBuffer vkCommandBuffer) {
            this->vkCommandBuffer = vkCommandBuffer;
        }

        PipelineInstance *getPipeline() {
            return pipelineInstance;
        }

        virtual void bindStaticDescriptorSets();

        void bindDescriptorSets(const std::vector<VkDescriptorSet> &descriptors);

        void bindSingleDescriptorSet(unsigned int descriptorSetIndex, const VkDescriptorSet &descriptorSet);

        VkPipelineBindPoint getBindingPoint() const;
    };
} // Metal

#endif //ABSTRACTPASS_H
