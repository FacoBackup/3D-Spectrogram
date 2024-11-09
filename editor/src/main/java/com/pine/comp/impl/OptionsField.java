package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.engine.inspection.SelectableEnum;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;

import java.util.function.BiConsumer;

public class OptionsField extends AbstractFormField {
    private SelectableEnum selected;
    public OptionsField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        selected = (SelectableEnum) dto.getValue();
    }

    @Override
    public void render() {
        ImGui.text(dto.getLabel());
        for(var op : dto.getOptions()){
            if(ImGui.checkbox(op.getTitle(), selected == op)){
                selected = op;
                changerHandler.accept(dto, op);
            }
        }
    }
}
