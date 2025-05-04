#include "InformationPanel.h"

namespace Metal {
    void InformationPanel::onSync() {
        for (const auto &item: colorLabels) {
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            ImVec2 p0 = ImGui::GetCursorScreenPos();
            ImVec2 p1 = ImVec2(p0.x + 35.f, p0.y + 5.0f);

            draw_list->AddRectFilled(p0, p1, ImColor(item.color));
            ImGui::Dummy(ImVec2(40.f, 5.0f));

            ImGui::SameLine();
            ImGui::Text("%s", item.label.c_str());
        }
    }
} // Metal
