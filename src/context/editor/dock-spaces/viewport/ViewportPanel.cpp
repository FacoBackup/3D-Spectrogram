#include "ViewportPanel.h"

#include "../../../../context/ApplicationContext.h"
#include "../../../../service/framebuffer/FrameBufferInstance.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <sndfile.h>
#include <vector>

#include "../../../../util/UIUtil.h"
#include "components/CameraGizmoPanel.h"


namespace Metal {
    void ViewportPanel::renderSpectrogramAxisLabels() const {
        for (int i = 0; i <= context->globalRepository.maxZAxis; i += 2) {
            std::ostringstream ss;
            ss.precision(2);
            ss << std::fixed << (i * WORLD_VOXEL_SCALE/2.f);
            std::string label = ss.str() + "hz";

            UIUtil::Draw3DLabel(
                {context->globalRepository.maxXAxis, 0, i},
                label.c_str(),
                context->engineContext.camera.projViewMatrix,
                IM_COL32(0, 0, 255, 255)
            );
        }
        for (int i = 2; i <= context->globalRepository.maxYAxis; i += 2) {
            UIUtil::Draw3DLabel(
                {
                    context->globalRepository.maxXAxis,
                    i,
                    context->globalRepository.maxZAxis
                },
                (std::to_string(i)).c_str(),
                context->engineContext.camera.projViewMatrix,
                IM_COL32(0, 128, 0, 255)
            );
        }
        for (int i = 0; i <= context->globalRepository.maxXAxis; i += 2) {
            UIUtil::Draw3DLabel({
                                    i, 0,
                                    context->globalRepository.maxZAxis
                                },
                                (std::to_string(i) + "s").c_str(),
                                context->engineContext.camera.projViewMatrix,
                                IM_COL32(255, 0, 0, 255)
            );
        }
    }

    void ViewportPanel::renderOriginalWaveAxisLabels() const {
        for (float i = 0; i <= SAMPLE_SIZE_SECONDS / 2.f; i += 0.2f) {
            std::ostringstream ss;
            ss.precision(2);
            ss << std::fixed << (i + context->spectrogramRepository.
                                 rangeStart);

            std::string label = ss.str() + "s";

            UIUtil::Draw3DLabel(
                {
                    (i) * ORIGINAL_WAVE_SCALE,
                    0,
                    context->globalRepository.maxZAxis
                },
                label.c_str(),
                context->engineContext.camera.projViewMatrix,
                IM_COL32_BLACK
            );
        }
    }

    void ViewportPanel::renderCameraModeToggle() const {
        const char *buttonLabel = !context->globalRepository.isOrthographic ? "Perspectiva" : "Ortográfica";
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize(buttonLabel).x - 24, GIZMO_SIZE + 8));
        if (ImGui::Button(buttonLabel)) {
            context->globalRepository.isOrthographic = !context->globalRepository.isOrthographic;
            if (context->engineContext.camera.isOrthographic != context->globalRepository.isOrthographic) {
                context->engineContext.camera.isOrthographic = context->globalRepository.isOrthographic;
                context->cameraService.updateCameraTarget();
            }
        }
    }

    void ViewportPanel::renderFrame() const {
        const ImVec2 windowSize = ImGui::GetWindowSize();
        auto *framebuffer = context->coreFrameBuffers.imageFBO;
        context->descriptorService.setImageDescriptor(framebuffer, 0);
        ImGui::Image(reinterpret_cast<ImTextureID>(framebuffer->attachments[0]->imageDescriptor->vkDescriptorSet),
                     windowSize);
    }

    void ViewportPanel::renderStaticCurveLabels() const {
        for (int i = 0; i <= context->globalRepository.maxXAxis; i++) {
            UIUtil::Draw3DLabel(
                {i, 0, 0},
                std::to_string(i).c_str(),
                context->engineContext.camera.projViewMatrix, IM_COL32(128, 0, 0, 255)
            );
        }
        for (int i = 0; i <= context->globalRepository.maxYAxis; i++) {
            UIUtil::Draw3DLabel(
                {0, i, 0},
                std::to_string(i).c_str(),
                context->engineContext.camera.projViewMatrix,IM_COL32(0, 128, 0, 255)
            );
        }
        for (int i = 0; i <= context->globalRepository.maxZAxis; i++) {
            UIUtil::Draw3DLabel(
                {0, 0, i},
                std::to_string(i).c_str(),
                context->engineContext.camera.projViewMatrix, IM_COL32(0, 0, 128, 255)
            );
        }
    }

    void ViewportPanel::renderContent() const {
        context->controlService.onSync();

        renderFrame();
        onSyncChildren();
        renderCameraModeToggle();

        if (!context->globalRepository.isOrthographic) {
            return;
        }

        if (context->spectrogramRepository.isShowingOriginalWave) {
            renderOriginalWaveAxisLabels();
            return;
        }
        if (context->globalRepository.isShowStaticCurve) {
            renderStaticCurveLabels();
            return;
        }
        if (!context->spectrogramRepository.pathToAudio.empty()) {
            renderSpectrogramAxisLabels();
        }
    }

    void ViewportPanel::onInitialize() {
        appendChild(new CameraGizmoPanel);
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
}
