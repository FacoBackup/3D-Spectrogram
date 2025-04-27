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

    void TimeStampPickerPanel::handleDrag(float timelineWidth) const {
        float delta = ImGui::GetIO().MouseDelta.x;
        float deltaTime = (delta / timelineWidth) * context->editorRepository.selectedAudioSize;

        context->editorRepository.rangeStart += deltaTime;
        context->editorRepository.rangeEnd += deltaTime;

        float rangeLength = context->editorRepository.rangeEnd - context->editorRepository.rangeStart;
        if (context->editorRepository.rangeStart < 0.0f) {
            context->editorRepository.rangeStart = 0.0f;
            context->editorRepository.rangeEnd = rangeLength;
        }
        if (context->editorRepository.rangeEnd > context->editorRepository.selectedAudioSize) {
            context->editorRepository.rangeEnd = context->editorRepository.selectedAudioSize;
            context->editorRepository.rangeStart = context->editorRepository.rangeEnd - rangeLength;
        }
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

        ImVec2 pMin = ImVec2(xStart, canvasPos.y);
        ImVec2 pMax = ImVec2(xEnd, canvasEnd.y);
        drawList->AddRectFilled(pMin, pMax, IM_COL32(0, 200, 0, 100));
        drawList->AddRect(pMin, pMax, IM_COL32(0, 200, 0, 255), 0.0f, 0, 2.0f);

        ImGui::SetCursorScreenPos(ImVec2(xStart, canvasPos.y));
        ImVec2 rangeSize(xEnd - xStart, canvasSize.y);
        ImGui::InvisibleButton(("##RangeDrag" + id).c_str(), rangeSize);

        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            handleDrag(timelineWidth);
        }

        ImGui::Text("%.2fs até %.2fs", context->editorRepository.rangeStart,
                    context->editorRepository.rangeEnd);
    }
} // Metal
