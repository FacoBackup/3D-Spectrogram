#include "ApplicationContext.h"

#include <imgui_impl_glfw.h>
#include <nfd.h>

#include "../util/FilesUtil.h"
#include "../util/VulkanUtils.h"

#include "../util/FileDialogUtil.h"

namespace Metal {
    uint32_t ApplicationContext::getFrameIndex() const {
        return vulkanContext.imguiVulkanWindow.FrameIndex;
    }

    void ApplicationContext::start() {
        NFD_Init();
        FilesUtil::MkDir(getShadersDirectory());

        glfwContext.onInitialize();
        if (!glfwContext.isValidContext()) {
            throw std::runtime_error("Could not create window");
        }
        vulkanContext.onInitialize();
        guiContext.onInitialize();
        editorPanel.onInitialize();
        engineContext.onInitialize();

        GLFWwindow *window = glfwContext.getWindow();
        while (!glfwWindowShouldClose(window)) {
            if (glfwContext.beginFrame()) {
                GuiContext::BeginFrame();
                try {
                    editorPanel.onSync();
                } catch (std::exception &e) {

                }
                ImGui::Render();
                auto *drawData = ImGui::GetDrawData();
                const bool main_is_minimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
                if (!main_is_minimized) {
                    vulkanContext.getCommandBuffers().clear();
                    auto &wd = vulkanContext.imguiVulkanWindow;
                    VkSemaphore imageAcquiredSemaphore = wd.FrameSemaphores[wd.SemaphoreIndex].ImageAcquiredSemaphore;
                    VkResult err = vkAcquireNextImageKHR(vulkanContext.device.device, wd.Swapchain, UINT64_MAX,
                                                         imageAcquiredSemaphore, VK_NULL_HANDLE,
                                                         &wd.FrameIndex);
                    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
                        glfwContext.setSwapChainRebuild(true);
                        return;
                    }

                    VulkanUtils::CheckVKResult(err);
                    ImGui_ImplVulkanH_Frame *fd = &wd.Frames[getFrameIndex()];
                    VulkanUtils::CheckVKResult(vkWaitForFences(vulkanContext.device.device, 1, &fd->Fence, VK_TRUE,
                                                               UINT64_MAX));
                    VulkanUtils::CheckVKResult(vkResetFences(vulkanContext.device.device, 1, &fd->Fence));
                    VulkanUtils::CheckVKResult(vkResetCommandPool(vulkanContext.device.device, fd->CommandPool,
                                                                  VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
                    engineContext.onSync();
                    GuiContext::RecordImguiCommandBuffer(drawData, err, wd, fd);
                    vulkanContext.submitFrame(imageAcquiredSemaphore, wd.FrameSemaphores[wd.SemaphoreIndex].
                                              RenderCompleteSemaphore, fd);
                }
                if (!main_is_minimized)
                    glfwContext.presentFrame();
            }
        }
        NFD_Quit();
        guiContext.dispose();
        vulkanContext.dispose();
        glfwContext.dispose();
    }
}
