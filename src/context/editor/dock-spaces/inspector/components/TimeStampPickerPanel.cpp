#include "TimeStampPickerPanel.h"

#include "../../../../ApplicationContext.h"
#define PICKER_RADIUS 6.0f

namespace Metal {
    void TimeStampPickerPanel::renderBackground(float timelineWidth, ImDrawList *&drawList, ImVec2 &canvasPos,
                                                ImVec2 &canvasSize, ImVec2 &canvasEnd) {
        drawList = ImGui::GetWindowDrawList();
        canvasPos = ImGui::GetCursorScreenPos();
        canvasSize = ImVec2(timelineWidth, 40.0f);
        canvasEnd = {canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y};

        float midY = canvasPos.y + canvasSize.y * 0.5f;
        drawList->AddLine(ImVec2(canvasPos.x, midY), ImVec2(canvasEnd.x, midY), IM_COL32(200, 200, 200, 255), 2.0f);
    }

    void TimeStampPickerPanel::renderStartHandle(float timelineWidth,
                                                 ImDrawList *drawList, ImVec2 canvasPos, ImVec2 canvasSize, ImVec2,
                                                 float xStart) {
        float midY = canvasPos.y + canvasSize.y * 0.5f;

        ImGui::SetCursorScreenPos(ImVec2(xStart - PICKER_RADIUS, midY - PICKER_RADIUS));
        ImGui::InvisibleButton("##StartHandle", ImVec2(PICKER_RADIUS * 2, PICKER_RADIUS * 2));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            float delta = ImGui::GetIO().MouseDelta.x;
            context->editorRepository.rangeStart += (delta / timelineWidth) * context->editorRepository.selectedAudioSize;
            context->editorRepository.rangeStart = std::ranges::clamp(context->editorRepository.rangeStart, 0.0f,
                                                                      context->editorRepository.rangeEnd);
        }

        drawList->AddCircleFilled(ImVec2(xStart, midY), PICKER_RADIUS, IM_COL32(255, 255, 0, 255));
    }

    void TimeStampPickerPanel::renderEndHandle(float timelineWidth,
                                               ImDrawList *drawList, ImVec2 canvasPos, ImVec2 canvasSize, ImVec2,
                                               float xEnd) {
        float midY = canvasPos.y + canvasSize.y * 0.5f;

        ImGui::SetCursorScreenPos(ImVec2(xEnd - PICKER_RADIUS, midY - PICKER_RADIUS));
        ImGui::InvisibleButton("##EndHandle", ImVec2(PICKER_RADIUS * 2, PICKER_RADIUS * 2));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            float delta = ImGui::GetIO().MouseDelta.x;
            context->editorRepository.rangeEnd += (delta / timelineWidth) * context->editorRepository.selectedAudioSize;
            context->editorRepository.rangeEnd = std::ranges::clamp(context->editorRepository.rangeEnd,
                                                                    context->editorRepository.rangeStart,
                                                                    context->editorRepository.selectedAudioSize);
        }

        drawList->AddCircleFilled(ImVec2(xEnd, midY), PICKER_RADIUS, IM_COL32(255, 100, 0, 255));
    }

    void TimeStampPickerPanel::onSync() {
        float timelineWidth = ImGui::GetContentRegionAvail().x;
        ImGui::Text("Porção para análise");

        ImDrawList *drawList;
        ImVec2 canvasPos;
        ImVec2 canvasSize;
        ImVec2 canvasEnd;
        renderBackground(timelineWidth, drawList, canvasPos, canvasSize, canvasEnd);

        auto timeToScreenX = [&](float t) -> float {
            return canvasPos.x + (t / context->editorRepository.selectedAudioSize) * timelineWidth;
        };

        float xStart = timeToScreenX(context->editorRepository.rangeStart);
        float xEnd = timeToScreenX(context->editorRepository.rangeEnd);

        float midY = canvasPos.y + canvasSize.y * 0.5f;
        drawList->AddLine(ImVec2(xStart, midY), ImVec2(xEnd, midY), IM_COL32(0, 200, 0, 255), 2.0f);

        renderStartHandle(timelineWidth, drawList, canvasPos, canvasSize, canvasEnd, xStart);
        renderEndHandle(timelineWidth, drawList, canvasPos, canvasSize, canvasEnd, xEnd);

        ImGui::Text("%.2fs até %.2fs", context->editorRepository.rangeStart,
                    context->editorRepository.rangeEnd);
    }
} // Metal
