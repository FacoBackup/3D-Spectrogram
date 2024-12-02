package com.pine.impl;

import com.pine.comp.FormPanel;
import com.pine.engine.injection.PInject;
import com.pine.engine.service.rendering.RequestProcessingService;
import com.pine.engine.service.request.UpdateFieldRequest;
import com.pine.engine.service.svo.VoxelService;
import imgui.ImGui;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import java.io.File;

public class InspectorPanel extends AbstractView implements Loggable {
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

        if (ImGui.button("Importar audio")) {
            voxelService.buildFromScratch();
        }
        formPanel.setInspectable(voxelService.voxelRepository);
        super.render();
    }

}
