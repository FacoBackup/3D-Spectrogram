package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;

import java.util.function.BiConsumer;

public class IntField extends AbstractFormField {
    private final int[] values = new int[1];

    public IntField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        var cast = (Integer) dto.getValue();
        values[0] = cast;
    }

    @Override
    public void render() {
        ImGui.text(dto.getLabel());
        if (ImGui.dragInt(imguiId, values, 1, dto.getMin(), dto.getMax())) {
            changerHandler.accept(dto, values[0]);
        }
    }
}
