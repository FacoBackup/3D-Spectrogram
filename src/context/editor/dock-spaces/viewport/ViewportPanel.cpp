#include "ViewportPanel.h"

#include "../../../../context/ApplicationContext.h"
#include "../../../../service/framebuffer/FrameBufferInstance.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>
#include <sndfile.h>
#include <vector>
#include <glm/vec3.hpp>

namespace Metal {
    void ViewportPanel::onSync() {
        updateInputs();
        updateCamera();
        const ImVec2 windowSize = ImGui::GetWindowSize();
        auto *framebuffer = context->coreFrameBuffers.imageFBO;
        context->descriptorService.setImageDescriptor(framebuffer, 0);
        ImGui::Image(reinterpret_cast<ImTextureID>(framebuffer->attachments[0]->imageDescriptor->vkDescriptorSet),
                     windowSize);
        renderCameraGizmo();
    }

    void ViewportPanel::renderCameraGizmo() {
        auto &camera = context->engineContext.camera;

        ImGuizmo::SetDrawlist();

        ImVec2 viewportPos = ImGui::GetWindowPos();
        ImVec2 viewportSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

        float gizmoSize = 100.0f;
        ImVec2 gizmoPos = ImVec2(viewportPos.x, viewportPos.y);

        float *v = glm::value_ptr(camera.viewMatrix);
        glm::mat4 originalView = camera.viewMatrix;

        ImGuizmo::ViewManipulate(v, 4.0f, gizmoPos, ImVec2(gizmoSize, gizmoSize), IM_COL32(40, 40, 40, 0));

        if (camera.viewMatrix != originalView) {
            camera.invViewMatrix = glm::inverse(camera.viewMatrix);
            camera.projViewMatrix = camera.projectionMatrix * camera.viewMatrix;
            camera.invProjectionMatrix = glm::inverse(camera.projectionMatrix);
            camera.position = glm::vec3(camera.invViewMatrix[3]);

            glm::vec3 forward = glm::normalize(glm::vec3(-camera.viewMatrix[2]));
            camera.yaw = atan2(forward.x, forward.z);
            camera.pitch = asin(forward.y);
        }
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
            } else {
                isFirstMovement = true;
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
