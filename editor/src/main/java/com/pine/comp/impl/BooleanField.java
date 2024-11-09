package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;

import java.util.function.BiConsumer;

public class BooleanField extends AbstractFormField {
    private boolean value;

    public BooleanField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        value = (Boolean) dto.getValue();
    }

    @Override
    public void render() {
        if(ImGui.checkbox(dto.getLabel(), value)){
            value = !value;
            changerHandler.accept(dto, value);
        }
    }
}
