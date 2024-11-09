package com.pine.comp;

import com.pine.impl.AbstractView;
import com.pine.engine.inspection.FieldDTO;

import java.util.function.BiConsumer;

public abstract class AbstractFormField extends AbstractView {
    protected final FieldDTO dto;
    protected final BiConsumer<FieldDTO, Object> changerHandler;

    public AbstractFormField(FieldDTO dto, BiConsumer<FieldDTO, Object> changerHandler) {
        this.dto = dto;
        this.changerHandler = changerHandler;
    }
}
