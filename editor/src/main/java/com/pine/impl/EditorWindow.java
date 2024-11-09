package com.pine.impl;

import com.pine.engine.Engine;
import com.pine.engine.injection.PInject;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;


public class EditorWindow extends AbstractView {
    @PInject
    public Engine engine;

    @PInject
    public WindowService windowService;

    private boolean isInitialized = false;
    private AbstractView pViewport;
    private AbstractView pInspector;
    private boolean isFirst = true;

    @Override
    public void onInitialize() {
        super.onInitialize();
        appendChild(pViewport = new ViewportPanel());
        appendChild(pInspector = new InspectorPanel());
    }

    @Override
    public void render() {
        if (!isInitialized) {
            engine.start(windowService.getDisplayW(), windowService.getDisplayH());
            isInitialized = true;
        }
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(windowService.getDisplayW(), windowService.getDisplayH());
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, new ImVec2(0, 0));
        if (ImGui.begin("Janela##TCC", ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.AlwaysAutoResize)) {
            ImGui.columns(2);
            if(isFirst) {
                ImGui.setColumnWidth(0, windowService.getDisplayW() * .75f);
                isFirst = false;
            }
            if (ImGui.beginChild("viewport")) {
                pViewport.render();
            }
            ImGui.endChild();
            ImGui.nextColumn();

            if (ImGui.beginChild("inspector")) {
                pInspector.render();
            }
            ImGui.endChild();
        }
        ImGui.end();
        ImGui.columns(1);
        ImGui.popStyleVar();
    }
}