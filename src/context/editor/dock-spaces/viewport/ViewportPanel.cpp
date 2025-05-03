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

#include "../../../../util/UIUtil.h"

#define GIZMO_SIZE 100.0f

namespace Metal {
    void ViewportPanel::renderContent() {
        context->controlService.onSync();

        const ImVec2 windowSize = ImGui::GetWindowSize();
        auto *framebuffer = context->coreFrameBuffers.imageFBO;
        context->descriptorService.setImageDescriptor(framebuffer, 0);
        ImGui::Image(reinterpret_cast<ImTextureID>(framebuffer->attachments[0]->imageDescriptor->vkDescriptorSet),
                     windowSize);
        renderCameraGizmo();

        const char *buttonLabel = !context->editorRepository.isOrthographic ? "Perspectiva" : "Ortográfica";
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize(buttonLabel).x - 24, GIZMO_SIZE + 8));
        if (ImGui::Button(buttonLabel)) {
            context->editorRepository.isOrthographic = !context->editorRepository.isOrthographic;
            if (context->engineContext.camera.isOrthographic != context->editorRepository.isOrthographic) {
                context->engineContext.camera.isOrthographic = context->editorRepository.isOrthographic;
                context->cameraService.updateCameraTarget();
            }
        }
        if (context->editorRepository.isOrthographic) {
            for (int i = 1; i <= context->editorRepository.maxFrequency; i += 2) {
                int fq = context->editorRepository.maxFrequency - i;
                UIUtil::Draw3DLabel({context->editorRepository.sampleSize, 0, i}, (std::to_string(fq * 10) + "hz").c_str(),
                                    context->engineContext.camera.projViewMatrix);
            }

            for (int i = 1; i <= context->editorRepository.sampleSize; i += 2) {
                UIUtil::Draw3DLabel({context->editorRepository.sampleSize - i, 0, context->editorRepository.maxFrequency}, (std::to_string(i) + "s").c_str(),
                                    context->engineContext.camera.projViewMatrix);
            }
        }
    }

    void ViewportPanel::onSync() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("MainViewport##TCC", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

        renderContent();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ViewportPanel::renderCameraGizmo() {
        auto &camera = context->engineContext.camera;

        ImGuizmo::SetDrawlist();

        ImVec2 viewportPos = ImGui::GetWindowPos();
        ImVec2 viewportSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

        ImVec2 gizmoPos = ImVec2(viewportPos.x + viewportSize.x - GIZMO_SIZE, viewportPos.y);

        if (tempView != camera.viewMatrix && !isManipulating) {
            tempView = camera.viewMatrix;
        }
        ImGuizmo::ViewManipulate(glm::value_ptr(tempView), 4.0f, gizmoPos, ImVec2(GIZMO_SIZE, GIZMO_SIZE),
                                 IM_COL32(40, 40, 40, 0));
        ImGui::SetCursorPos(gizmoPos);
        ImGui::InvisibleButton("gizmoButton", ImVec2(GIZMO_SIZE, GIZMO_SIZE));

        isManipulating = camera.viewMatrix != tempView && ImGui::IsItemHovered();
        if (isManipulating) {
            glm::vec3 forward = glm::normalize(glm::inverse(tempView)[2]); // world-space forward

            camera.yaw = atan2(forward.x, forward.z);
            camera.pitch = asin(forward.y);

            camera.changed = true;
        }
    }
}
