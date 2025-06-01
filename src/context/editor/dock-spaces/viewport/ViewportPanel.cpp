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
        for (int i = 0; i <= context->editorRepository.maxZAxis; i += 2) {
            int fq = context->editorRepository.maxZAxis - i;
            UIUtil::Draw3DLabel(
                {context->editorRepository.maxXAxis, -1, i - 1},
                (std::to_string(fq * 50) + "hz").c_str(),
                context->engineContext.camera.projViewMatrix,
                IM_COL32(0, 0, 255, 255)
            );
        }
        for (int i = 2; i <= context->editorRepository.maxYAxis; i += 2) {
            int magnitude = context->editorRepository.maxYAxis - i;
            UIUtil::Draw3DLabel(
                {
                    context->editorRepository.maxXAxis,
                    i - 2,
                    context->editorRepository.maxZAxis
                },
                (std::to_string(magnitude)).c_str(),
                context->engineContext.camera.projViewMatrix,
                IM_COL32(0, 128, 0, 255)
            );
        }
        for (int i = 0; i <= static_cast<int>(context->editorRepository.maxXAxis); i += 2) {
            UIUtil::Draw3DLabel({
                                    context->editorRepository.maxXAxis - i, -1,
                                    context->editorRepository.maxZAxis
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
            ss << std::fixed << (context->editorRepository.rangeEnd - i + context->editorRepository.rangeStart);

            std::string label = ss.str() + "s";

            UIUtil::Draw3DLabel(
                {
                    (i) * ORIGINAL_WAVE_SCALE,
                    0,
                    context->editorRepository.maxZAxis
                },
                label.c_str(),
                context->engineContext.camera.projViewMatrix,
                IM_COL32_BLACK
            );
        }
    }

    void ViewportPanel::renderCameraModeToggle() const {
        const char *buttonLabel = !context->editorRepository.isOrthographic ? "Perspectiva" : "Ortográfica";
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize(buttonLabel).x - 24, GIZMO_SIZE + 8));
        if (ImGui::Button(buttonLabel)) {
            context->editorRepository.isOrthographic = !context->editorRepository.isOrthographic;
            if (context->engineContext.camera.isOrthographic != context->editorRepository.isOrthographic) {
                context->engineContext.camera.isOrthographic = context->editorRepository.isOrthographic;
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
        for (int i = 1; i <= context->editorRepository.maxXAxis; i += 1) {
            UIUtil::Draw3DLabel(
                {i, context->editorRepository.maxYAxis, context->editorRepository.maxZAxis},
                std::to_string(context->editorRepository.maxXAxis - i).c_str(),
                context->engineContext.camera.projViewMatrix, IM_COL32(255, 0, 0, 255)
            );
        }
        for (int i = 1; i <= context->editorRepository.maxYAxis; i += 1) {
            UIUtil::Draw3DLabel(
                {context->editorRepository.maxXAxis, i - 1.5f, context->editorRepository.maxZAxis},
                std::to_string(context->editorRepository.maxYAxis - i).c_str(),
                context->engineContext.camera.projViewMatrix,IM_COL32(0, 128, 0, 255)
            );
        }
        for (int i = 1; i <= context->editorRepository.maxZAxis; i += 1) {
            UIUtil::Draw3DLabel(
                {context->editorRepository.maxXAxis, context->editorRepository.maxYAxis, i - 1},
                std::to_string(context->editorRepository.maxZAxis - i).c_str(),
                context->engineContext.camera.projViewMatrix, IM_COL32(0, 0, 255, 255)
            );
        }
    }

    void ViewportPanel::renderContent() const {
        context->controlService.onSync();

        renderFrame();
        onSyncChildren();
        renderCameraModeToggle();

        if (!context->editorRepository.isOrthographic) {
            return;
        }

        if (context->editorRepository.isShowingOriginalWave) {
            renderOriginalWaveAxisLabels();
            return;
        }
        if (context->editorRepository.isShowStaticCurve) {
            renderStaticCurveLabels();
            return;
        }
        if (!context->editorRepository.pathToAudio.empty()) {
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
