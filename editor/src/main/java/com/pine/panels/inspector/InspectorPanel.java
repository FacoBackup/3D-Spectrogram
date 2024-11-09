package com.pine.panels.inspector;

import com.pine.component.AbstractComponent;
import com.pine.component.ComponentType;
import com.pine.component.Entity;
import com.pine.core.dock.AbstractDockPanel;
import com.pine.injection.PInject;
import com.pine.inspection.Inspectable;
import com.pine.panels.component.FormPanel;
import com.pine.repository.EditorRepository;
import com.pine.repository.WorldRepository;
import com.pine.repository.fs.ResourceEntry;
import com.pine.service.rendering.RequestProcessingService;
import com.pine.service.request.AddComponentRequest;
import com.pine.service.request.UpdateFieldRequest;
import com.pine.service.svo.VoxelService;
import com.pine.theme.Icons;
import imgui.ImGui;
import imgui.flag.ImGuiCol;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.stream.Stream;

import static com.pine.theme.Icons.ONLY_ICON_BUTTON_SIZE;

public class InspectorPanel extends AbstractDockPanel {
    @PInject
    public RequestProcessingService requestProcessingService;
    @PInject
    public EditorRepository editorRepository;
    @PInject
    public List<Inspectable> repositories;
    @PInject
    public VoxelService voxelService;

    public Inspectable selectedFile;

    private final List<Inspectable> additionalInspectable = new ArrayList<>();

    private Inspectable currentInspection;
    private Entity selected;
    private final List<String> types = new ArrayList<>();
    private FormPanel formPanel;
    private ResourceEntry previousSelectedEntry;

    @Override
    public void onInitialize() {
        types.add(Icons.add + " Add component");
        types.addAll(Stream.of(ComponentType.values()).map(a -> a.getIcon() + a.getTitle()).toList());
        formPanel = appendChild(new FormPanel((dto, newValue) -> requestProcessingService.addRequest(new UpdateFieldRequest(dto, newValue))));
        currentInspection = repositories.getFirst();
    }

    @Override
    public void render() {
        if (ImGui.button(Icons.apps + "Repackage scene voxels##vp")) {
            voxelService.buildFromScratch();
        }
        formPanel.setInspectable(voxelService.voxelRepository);
        super.render();
    }

}
