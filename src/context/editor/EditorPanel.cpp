#include "EditorPanel.h"

#include "NotificationPanel.h"
#include "../../util/UIUtil.h"
#include "../../context/ApplicationContext.h"
#include "dock-spaces/InformationPanel.h"
#include "dock-spaces/inspector/InspectorPanel.h"
#include "dock-spaces/viewport/ViewportPanel.h"
#define MARGIN  8.0f

namespace Metal {
    void EditorPanel::renderInspector() const {
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        float inspectorWidth = 300.0f;
        float inspectorHeight = screenSize.y * 0.7f;
        float inspectorX = screenSize.x - inspectorWidth - MARGIN;

        ImGui::SetNextWindowPos(ImVec2(inspectorX, MARGIN), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(inspectorWidth, inspectorHeight), ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
        if (ImGui::Begin("Inspetor", nullptr, ImGuiWindowFlags_NoResize)) {
            pInspector->onSync();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorPanel::renderInformation() {
        float informationWidth = 250.f;
        float informationHeight = 95.f;
        ImGui::SetNextWindowPos(
            ImVec2(MARGIN, ImGui::GetIO().DisplaySize.y - MARGIN - (isInfoCollapsed ? 25 : informationHeight)));
        if (!isInfoCollapsed)
            ImGui::SetNextWindowSize(ImVec2(informationWidth, informationHeight)); {
        }
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
        if (ImGui::Begin("Informações", nullptr, ImGuiWindowFlags_NoResize)) {
            pInformation->onSync();
            isInfoCollapsed = false;
        } else {
            isInfoCollapsed = true;
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

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

        pViewport->onSync();

        renderInspector();

        renderInformation();

        pNotifications->onSync();
    }

    void EditorPanel::onInitialize() {
        pViewport = new ViewportPanel;
        pInspector = new InspectorPanel;
        pNotifications = new NotificationPanel;
        pInformation = new InformationPanel;
        appendChild(pInformation);
        appendChild(pViewport);
        appendChild(pInspector);
        appendChild(pNotifications);
    }
}
