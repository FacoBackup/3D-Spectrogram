package com.pine.impl;

import com.pine.comp.FormPanel;
import com.pine.engine.injection.PInject;
import com.pine.engine.service.rendering.RequestProcessingService;
import com.pine.engine.service.request.UpdateFieldRequest;
import com.pine.engine.service.svo.VoxelService;
import imgui.ImGui;

public class InspectorPanel extends AbstractView {
    @PInject
    public RequestProcessingService requestProcessingService;
    @PInject
    public NativeDialogService nativeDialogService;
    @PInject
    public VoxelService voxelService;

    private FormPanel formPanel;

    @Override
    public void onInitialize() {
        formPanel = appendChild(new FormPanel((dto, newValue) -> requestProcessingService.addRequest(new UpdateFieldRequest(dto, newValue))));
    }

    @Override
    public void render() {
        if (ImGui.button(Icons.apps + "Computar voxels##vp")) {
            voxelService.buildFromScratch();
        }

//        if (ImGui.button("Importar audio")) {
//            var file = nativeDialogService.selectFile().stream().findFirst().orElse(null);

//        }
        formPanel.setInspectable(voxelService.voxelRepository);
        super.render();
    }

}
