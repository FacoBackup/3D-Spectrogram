#include "ThemeService.h"

#include "../../context/ApplicationContext.h"

namespace Metal {
    glm::vec3 ThemeService::BACKGROUND_COLOR = glm::vec3(0.f);

    void ThemeService::onSync() {
        if (isDone) return;
        isDone = true;

        ImGuiStyle &style = ImGui::GetStyle();
        auto &colors = style.Colors;

        ImGui::StyleColorsLight();
        palette0 = ImVec4(10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.0f);
        palette1 = ImVec4(18.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, .1f);
        palette2 = ImVec4(225.0f / 255.0f, 225.0f / 255.0f, 225.0f / 255.0f, 1.0f); // medium gray
        neutralPalette = palette2;
        palette3 = ImVec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f); // darker gray
        palette4 = ImVec4(160.0f / 255.0f, 160.0f / 255.0f, 160.0f / 255.0f, 1.0f); // even darker gray
        palette5 = ImVec4(120.0f / 255.0f, 120.0f / 255.0f, 120.0f / 255.0f, 1.0f); // dark gray
        palette6 = ImVec4(10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.0f); // very dark gray

        textDisabled = ImVec4(palette6.x / 2.f, palette6.y / 2.f, palette6.z / 2.f, 1);
        colors[ImGuiCol_Text] = palette0;
        colors[ImGuiCol_TextDisabled] = textDisabled;
        colors[ImGuiCol_WindowBg] = palette1;
        colors[ImGuiCol_ChildBg] = palette1;
        colors[ImGuiCol_PopupBg] = palette1;
        colors[ImGuiCol_Border] = palette1;
        colors[ImGuiCol_BorderShadow] = ImVec4(palette1.x, palette1.y, palette1.z, .5f);
        colors[ImGuiCol_FrameBg] = palette2;
        colors[ImGuiCol_TitleBg] = palette1;
        colors[ImGuiCol_TitleBgActive] = palette1;
        colors[ImGuiCol_TitleBgCollapsed] = palette1;
        colors[ImGuiCol_MenuBarBg] = palette0;
        colors[ImGuiCol_ScrollbarBg] = palette1;
        colors[ImGuiCol_ScrollbarGrab] = palette3;
        colors[ImGuiCol_ScrollbarGrabHovered] = palette4;
        colors[ImGuiCol_ScrollbarGrabActive] = palette2;
        colors[ImGuiCol_SliderGrab] = palette4;
        colors[ImGuiCol_ButtonActive] = palette2;
        colors[ImGuiCol_Separator] = palette5;
        colors[ImGuiCol_SeparatorHovered] = palette6;
        colors[ImGuiCol_SeparatorActive] = palette6;
        colors[ImGuiCol_ResizeGrip] = palette4;
        colors[ImGuiCol_Tab] = palette2;
        colors[ImGuiCol_TabHovered] = palette3;
        colors[ImGuiCol_DockingPreview] = palette4;
        colors[ImGuiCol_DockingEmptyBg] = palette6;
        colors[ImGuiCol_PlotLines] = palette5;
        colors[ImGuiCol_PlotLinesHovered] = palette6;
        colors[ImGuiCol_PlotHistogram] = palette5;
        colors[ImGuiCol_PlotHistogramHovered] = palette6;
        colors[ImGuiCol_DragDropTarget] = palette4;
        colors[ImGuiCol_NavHighlight] = palette3;
        colors[ImGuiCol_NavWindowingHighlight] = palette2;
        colors[ImGuiCol_NavWindowingDimBg] = palette2;
        colors[ImGuiCol_ModalWindowDimBg] = palette2;
        ImVec4 accent{};
        accent.y = accentColor.y;
        accent.z = accentColor.z;
        accent.x = accentColor.x;
        accent.w = 1;

        colors[ImGuiCol_FrameBgHovered] = accent;
        colors[ImGuiCol_FrameBgActive] = accent;
        colors[ImGuiCol_CheckMark] = accent;
        colors[ImGuiCol_SliderGrabActive] = accent;
        colors[ImGuiCol_Button] = palette3;
        colors[ImGuiCol_ButtonHovered] = accent;
        colors[ImGuiCol_Header] = accent;
        colors[ImGuiCol_HeaderHovered] = accent;
        colors[ImGuiCol_HeaderActive] = accent;
        colors[ImGuiCol_ResizeGripHovered] = accent;
        colors[ImGuiCol_ResizeGripActive] = accent;
        colors[ImGuiCol_TextSelectedBg] = accent;

        BACKGROUND_COLOR[0] = colors[ImGuiCol_WindowBg].x;
        BACKGROUND_COLOR[1] = colors[ImGuiCol_WindowBg].y;
        BACKGROUND_COLOR[2] = colors[ImGuiCol_WindowBg].z;
    }
} // Metal
