#include "CoreFrameBuffers.h"
#include "../../context/ApplicationContext.h"


namespace Metal {
    void CoreFrameBuffers::onInitialize() {
        imageFBO = framebufferService.createFrameBuffer(vulkanContext.getWindowWidth(),
                                                                 vulkanContext.getWindowHeight());
        framebufferService.createAttachment("Color", VK_FORMAT_R16G16B16A16_SFLOAT,
                                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, imageFBO);
        framebufferService.createRenderPass(imageFBO);
    }
}
