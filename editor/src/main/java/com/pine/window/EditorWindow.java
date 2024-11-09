package com.pine.window;

import com.pine.Engine;
import com.pine.core.AbstractWindow;
import com.pine.core.WindowService;
import com.pine.core.dock.*;
import com.pine.core.view.AbstractView;
import com.pine.injection.PInject;
import com.pine.panels.ToasterPanel;
import com.pine.panels.inspector.InspectorPanel;
import com.pine.panels.viewport.ViewportPanel;
import com.pine.repository.EditorRepository;
import com.pine.service.ThemeService;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.ImVec4;
import imgui.flag.ImGuiKey;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import imgui.type.ImBoolean;

import java.util.Collections;
import java.util.List;

import static com.pine.core.dock.DockPanel.FLAGS;
import static com.pine.core.dock.DockPanel.OPEN;


public class EditorWindow extends AbstractWindow {
    private static final int FLAGS = ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.MenuBar;
    public static final ImBoolean OPEN = new ImBoolean(true);

    @PInject
    public Engine engine;

    @PInject
    public DockService dockService;

    @PInject
    public ThemeService themeService;

    @PInject
    public EditorRepository editorRepository;

    @PInject
    public WindowService windowService;

    private boolean isInitialized = false;
    private AbstractView pViewport;
    private AbstractView pInspector;

    @Override
    public void onInitialize() {
        super.onInitialize();
        appendChild(pViewport = new ViewportPanel());
        appendChild(pInspector = new InspectorPanel());
    }

    @Override
    public ImVec4 getNeutralPalette() {
        return themeService.neutralPalette;
    }

    @Override
    public ImVec4 getAccentColor() {
        return editorRepository.accent;
    }

    @Override
    public void render() {
        if (!isInitialized) {
            engine.start(windowService.getDisplayW(), windowService.getDisplayH());
            isInitialized = true;
        }
        themeService.tick();
        ImGui.setNextWindowPos(0, 0);
        ImGui.setNextWindowSize(windowService.getDisplayW(), windowService.getDisplayH());
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, new ImVec2(0, 0));
        if (ImGui.begin("Janela##TCC", ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.AlwaysAutoResize)) {
            ImGui.columns(2);
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
        ImGui.popStyleVar();
    }
}