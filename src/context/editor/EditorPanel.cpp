#include "EditorPanel.h"

#include "NotificationPanel.h"
#include "../../util/UIUtil.h"
#include "../../context/ApplicationContext.h"
#include "dock-spaces/inspector/InspectorPanel.h"
#include "dock-spaces/viewport/ViewportPanel.h"
#define MARGIN  8.0f

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
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("MainViewport##TCC", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

        pViewport->onSync();
        ImGui::End();
        ImGui::PopStyleVar();

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        float inspectorWidth = 300.0f;
        float inspectorHeight = screenSize.y * 0.7f;
        float inspectorX = screenSize.x - inspectorWidth - MARGIN;

        ImGui::SetNextWindowPos(ImVec2(inspectorX, MARGIN), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(inspectorWidth, inspectorHeight), ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
        if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize)) {
            pInspector->onSync();
        }
        ImGui::End();
        ImGui::PopStyleVar();

        pNotifications->onSync();
    }

    void EditorPanel::onInitialize() {
        pViewport = new ViewportPanel();
        pInspector = new InspectorPanel();
        pNotifications = new NotificationPanel;
        appendChild(pViewport);
        appendChild(pInspector);
        appendChild(pNotifications);
    }
}
