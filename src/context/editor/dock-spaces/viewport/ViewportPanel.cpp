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
            if (context->editorRepository.isShowingOriginalWave) {
                for (float i = 0; i <= SAMPLE_SIZE_SECONDS / 2.f; i += 0.2f) {
                    std::ostringstream ss;
                    ss.precision(2);
                    ss << std::fixed << (context->editorRepository.rangeEnd - i + context->editorRepository.rangeStart);

                    std::string label = ss.str() + "s";

                    UIUtil::Draw3DLabel(
                        {
                            (i) * ORIGINAL_WAVE_SCALE,
                            0,
                            context->editorRepository.maxFrequency
                        },
                        label.c_str(),
                        context->engineContext.camera.projViewMatrix,
                        IM_COL32_BLACK
                    );
                }
            } else if (!context->editorRepository.pathToAudio.empty()) {
                for (int i = 0; i <= context->editorRepository.maxFrequency; i += 2) {
                    int fq = context->editorRepository.maxFrequency - i;
                    UIUtil::Draw3DLabel(
                        {context->editorRepository.selectedAudioSize, -1, i - 1},
                        (std::to_string(fq * 50) + "hz").c_str(),
                        context->engineContext.camera.projViewMatrix,
                        IM_COL32(0, 0, 255, 255)
                    );
                }
                for (int i = 2; i <= context->editorRepository.maxMagnitude; i += 2) {
                    int magnitude = context->editorRepository.maxMagnitude - i;
                    UIUtil::Draw3DLabel(
                        {
                            context->editorRepository.selectedAudioSize,
                            i - 2,
                            context->editorRepository.maxFrequency
                        },
                        (std::to_string(magnitude)).c_str(),
                        context->engineContext.camera.projViewMatrix,
                        IM_COL32(0, 128, 0, 255)
                    );
                }
                for (int i = 0; i <= static_cast<int>(context->editorRepository.selectedAudioSize); i += 2) {
                    UIUtil::Draw3DLabel({
                                            context->editorRepository.selectedAudioSize - i, -1,
                                            context->editorRepository.maxFrequency
                                        },
                                        (std::to_string(i) + "s").c_str(),
                                        context->engineContext.camera.projViewMatrix,
                                        IM_COL32(255, 0, 0, 255)
                    );
                }
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
