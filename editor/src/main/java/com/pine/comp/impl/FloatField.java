package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;

import java.util.function.BiConsumer;

public class FloatField extends AbstractFormField {
    private final float[] values = new float[1];

    public FloatField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        var cast = (Float) dto.getValue();
        values[0] = cast;
    }

    @Override
    public void render() {
        ImGui.text(dto.getLabel());
        if(ImGui.dragFloat(imguiId, values, .01f, dto.getMin(), dto.getMax())){
            changerHandler.accept(dto, values[0]);
        }
    }
}
