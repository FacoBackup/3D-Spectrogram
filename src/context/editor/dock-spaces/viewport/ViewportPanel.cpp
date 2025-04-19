#include "ViewportPanel.h"

#include "CameraPositionPanel.h"
#include "ImGuizmo.h"
#include "../../../../context/ApplicationContext.h"
#include "../../../../service/framebuffer/FrameBufferInstance.h"

namespace Metal {
    void ViewportPanel::onInitialize() {
        appendChild(cameraPanel = new CameraPositionPanel());
    }

    void ViewportPanel::onSync() {
        updateCamera();
        updateInputs();
        const ImVec2 windowSize = ImGui::GetWindowSize();

        auto *framebuffer = context->coreFrameBuffers.imageFBO;
        context->descriptorService.setImageDescriptor(framebuffer, 0);
        ImGui::Image(reinterpret_cast<ImTextureID>(framebuffer->attachments[0]->imageDescriptor->vkDescriptorSet),
                     windowSize);

        cameraPanel->onSync();
    }

    void ViewportPanel::updateCamera() {
        auto &engineContext = context->engineContext;
        const auto &cameraService = context->cameraService;

        if (ImGui::IsWindowHovered() && !ImGuizmo::IsUsing()) {
            if (const auto &io = ImGui::GetIO(); io.MouseWheel != 0) {
                if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
                    engineContext.camera.movementSensitivity += io.MouseWheel * 100 * context->engineContext.deltaTime;
                    engineContext.camera.movementSensitivity =
                            std::max(.1f, engineContext.camera.movementSensitivity);
                } else {
                    cameraService.handleScroll(io.MouseWheel);
                }
            }
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                cameraService.handleInput(isFirstMovement);
                isFirstMovement = false;
            }
        } else {
            isFirstMovement = true;
        }
    }

    void ViewportPanel::updateInputs() const {
        auto &repo = context->runtimeRepository;
        const ImVec2 windowSize = ImGui::GetWindowSize();

        repo.viewportH = windowSize.y;
        repo.viewportW = windowSize.x;

        const ImVec2 windowPos = ImGui::GetWindowPos();
        repo.viewportX = windowPos.x;
        repo.viewportY = windowPos.y;

        repo.isFocused = ImGui::IsWindowHovered();
        repo.forwardPressed = ImGui::IsKeyDown(ImGuiKey_W);
        repo.backwardPressed = ImGui::IsKeyDown(ImGuiKey_S);
        repo.leftPressed = ImGui::IsKeyDown(ImGuiKey_A);
        repo.rightPressed = ImGui::IsKeyDown(ImGuiKey_D);
        repo.upPressed = ImGui::IsKeyDown(ImGuiKey_Space);
        repo.downPressed = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
        repo.mousePressed = ImGui::IsWindowFocused() && ImGui::IsWindowHovered() && ImGui::IsMouseDown(
                                ImGuiMouseButton_Left);

        const ImVec2 mousePos = ImGui::GetMousePos();
        repo.mouseX = mousePos.x;
        repo.mouseY = mousePos.y;

        repo.normalizedMouseX = (repo.mouseX + repo.viewportX) / repo.viewportW;
        repo.normalizedMouseY = (repo.viewportH - repo.mouseY + repo.viewportY) / repo.viewportH;
    }
}
