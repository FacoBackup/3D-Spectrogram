package com.pine.comp.impl;

import com.pine.engine.inspection.FieldDTO;
import com.pine.comp.AbstractFormField;
import imgui.ImGui;
import org.joml.Vector2f;

import java.util.function.BiConsumer;

public class Vector2Field extends AbstractFormField {
    private final float[] values = new float[4];
    private final Vector2f valVec2;

    public Vector2Field(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        super(dto, changerHandler);
        valVec2 = (Vector2f) dto.getValue();
    }

    @Override
    public void render() {
        values[0] = valVec2.x;
        values[1] = valVec2.y;

        ImGui.text(dto.getLabel());
        if (ImGui.dragFloat2(imguiId, values, .01f, dto.getMin(), dto.getMax())) {
            changerHandler.accept(dto, values);
        }
    }
}
