#include "AbstractPass.h"
#include "../../../context/ApplicationContext.h"
#include "../../../service/pipeline/PipelineInstance.h"

namespace Metal {
    AbstractPass::AbstractPass(ApplicationContext &context, bool isComputePass) : AbstractRuntimeComponent(context){
    }

    void AbstractPass::recordPushConstant(const void *data) {
        vkCmdPushConstants(
            vkCommandBuffer,
            getPipeline()->vkPipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            getPipeline()->pushConstantsSize,
            data);
    }

    void AbstractPass::bindStaticDescriptorSets() {
        bindDescriptorSets(getPipeline()->descriptorSets);
    }

    void AbstractPass::bindDescriptorSets(const std::vector<VkDescriptorSet> &descriptors) {
        if (!descriptors.empty()) {
            vkCmdBindDescriptorSets(vkCommandBuffer,
                                    getBindingPoint(),
                                    getPipeline()->vkPipelineLayout,
                                    0,
                                    descriptors.size(),
                                    descriptors.data(),
                                    0,
                                    nullptr);
        }
    }

    void AbstractPass::bindSingleDescriptorSet(const unsigned int descriptorSetIndex, const VkDescriptorSet &descriptorSet) {
        vkCmdBindDescriptorSets(
            vkCommandBuffer,
            getBindingPoint(),
            getPipeline()->vkPipelineLayout,
            descriptorSetIndex,
            1,
            &descriptorSet,
            0,
            nullptr
        );
    }

    VkPipelineBindPoint AbstractPass::getBindingPoint() const {
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    }
}
