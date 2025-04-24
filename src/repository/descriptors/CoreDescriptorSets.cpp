#include "CoreDescriptorSets.h"

#include <vulkan/vulkan_core.h>
#include "../../context/ApplicationContext.h"
#include "../../service/descriptor/DescriptorInstance.h"


namespace Metal {
    void CoreDescriptorSets::onInitialize() {
        svoData = std::make_unique<DescriptorInstance>();
        svoData->addLayoutBinding(VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0);
        svoData->create(vulkanContext);
        svoData->addBufferDescriptor(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, context.coreBuffers.svoData);
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
