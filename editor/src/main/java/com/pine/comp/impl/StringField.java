package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;
import imgui.type.ImString;

import java.util.function.BiConsumer;

public class StringField extends AbstractFormField {
    private final ImString value = new ImString();

    public StringField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        value.set(dto.getValue());
    }

    @Override
    public void render() {
        ImGui.text(dto.getLabel());
        if(ImGui.inputText(dto.getId(), value)){
            changerHandler.accept(dto, value.get());
        }
    }
}
