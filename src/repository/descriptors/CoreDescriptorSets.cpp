#include "CoreDescriptorSets.h"

#include <vulkan/vulkan_core.h>
#include "../../context/ApplicationContext.h"
#include "../../service/descriptor/DescriptorInstance.h"


namespace Metal {
    void CoreDescriptorSets::onInitialize() {
        constexpr auto COMPUTE_FRAGMENT_STAGES = static_cast<VkShaderStageFlagBits>(
            VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT);
        svoData = std::make_unique<DescriptorInstance>();
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);
        svoData->create(vulkanContext);
        svoData->write(vulkanContext);


        globalDataDescriptor = std::make_unique<DescriptorInstance>();
        globalDataDescriptor->addLayoutBinding(static_cast<VkShaderStageFlagBits>(
                                                   VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT |
                                                   VK_SHADER_STAGE_COMPUTE_BIT),
                                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);
        globalDataDescriptor->create(vulkanContext);
        globalDataDescriptor->addBufferDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                  context.coreBuffers.globalData);
        globalDataDescriptor->write(vulkanContext);
    }
} // Metal
