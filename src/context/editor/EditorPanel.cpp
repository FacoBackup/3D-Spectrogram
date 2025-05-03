#include "EditorPanel.h"

#include "NotificationPanel.h"
#include "../../util/UIUtil.h"
#include "../../context/ApplicationContext.h"
#include "dock-spaces/InformationPanel.h"
#include "dock-spaces/inspector/InspectorPanel.h"
#include "dock-spaces/viewport/ViewportPanel.h"
#define MARGIN  8.0f
#define CONTENT_WIDTH 300.0f
#define WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
#define INFO_HEIGHT 70

namespace Metal {
    void EditorPanel::renderInspector() {
        ImGui::SetNextWindowPos(ImVec2(MARGIN, MARGIN), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(CONTENT_WIDTH, inspectorHeight), ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
        if (ImGui::Begin("Inspetor", nullptr, WINDOW_FLAGS)) {
            pInspector->onSync();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorPanel::renderInformation() {
        ImGui::SetNextWindowPos(ImVec2(MARGIN, inspectorHeight + MARGIN * 2), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(CONTENT_WIDTH, INFO_HEIGHT), ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
        if (ImGui::Begin("Informações", nullptr, WINDOW_FLAGS)) {
            pInformation->onSync();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorPanel::onSync() {
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        inspectorHeight = screenSize.y - INFO_HEIGHT - MARGIN * 3;

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
