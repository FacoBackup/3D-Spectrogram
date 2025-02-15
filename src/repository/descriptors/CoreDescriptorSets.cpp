#include "CoreDescriptorSets.h"

#include <vulkan/vulkan_core.h>
#include "../../context/ApplicationContext.h"
#include "../../service/descriptor/DescriptorInstance.h"
#include "../../service/framebuffer/FrameBufferInstance.h"
#include "../../service/framebuffer/FrameBufferAttachment.h"
#include "../../service/texture/TextureInstance.h"


namespace Metal {
    void CoreDescriptorSets::onInitialize() {
        constexpr auto COMPUTE_FRAGMENT_STAGES = static_cast<VkShaderStageFlagBits>(
            VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT);
        svoData = std::make_unique<DescriptorInstance>();
        // ONE FOR EACH ADJACENT TILE AND ONE FOR THE CENTER TILE

        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 4);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 5);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 6);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 7);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 8);
        svoData->addLayoutBinding(COMPUTE_FRAGMENT_STAGES, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 9);

        svoData->create(vulkanContext);
        svoData->addBufferDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                     context.coreBuffers.tileInfo);
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
