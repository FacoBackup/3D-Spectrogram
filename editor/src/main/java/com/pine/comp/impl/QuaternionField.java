package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;
import org.joml.Quaternionf;

import java.util.function.BiConsumer;

public class QuaternionField extends AbstractFormField {
    private final float[] values = new float[4];
    private final Quaternionf valQuat;

    public QuaternionField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        valQuat = (Quaternionf) dto.getValue();
    }

    @Override
    public void render() {
        values[0] = valQuat.x;
        values[1] = valQuat.y;
        values[2] = valQuat.z;
        values[3] = valQuat.w;
        ImGui.text(dto.getLabel());
        if (ImGui.dragFloat4(imguiId, values, .01f, dto.getMin(), dto.getMax())) {
            changerHandler.accept(dto, values);
        }
    }
}
