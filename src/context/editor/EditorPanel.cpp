#include "EditorPanel.h"
#include "../../util/UIUtil.h"
#include "../../context/ApplicationContext.h"
#include "dock-spaces/inspector/InspectorPanel.h"
#include "dock-spaces/viewport/ViewportPanel.h"

namespace Metal {
    void EditorPanel::onSync() {
        context->themeService.onSync();
        const ImGuiViewport *viewport = ImGui::GetMainViewport();

        UIUtil::AUX_VEC2.x = viewport->Pos.x;
        UIUtil::AUX_VEC2.y = viewport->Pos.y;
        ImGui::SetNextWindowPos(UIUtil::AUX_VEC2);

        UIUtil::AUX_VEC2.x = viewport->Size.x;
        UIUtil::AUX_VEC2.y = viewport->Size.y;
        ImGui::SetNextWindowSize(UIUtil::AUX_VEC2);
        ImGui::SetNextWindowViewport(viewport->ID);


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin("Janela##TCC", &UIUtil::OPEN,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Columns(2);
            if (isFirst) {
                ImGui::SetColumnWidth(0, viewport->Size.x * .75f);
                isFirst = false;
            }
            if (ImGui::BeginChild("viewport")) {
                pViewport->onSync();
            }
            ImGui::EndChild();
            ImGui::NextColumn();

            if (ImGui::BeginChild("inspector")) {
                pInspector->onSync();
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::Columns(1);
        ImGui::PopStyleVar();
    }

    void EditorPanel::onInitialize() {
        pViewport = new ViewportPanel();
        pInspector = new InspectorPanel();
        appendChild(pViewport);
        appendChild(pInspector);
    }
}
