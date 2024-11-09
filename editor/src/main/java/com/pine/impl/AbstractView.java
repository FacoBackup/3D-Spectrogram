package com.pine.impl;

import com.pine.engine.injection.PInject;
import com.pine.engine.injection.PInjector;
import com.pine.engine.service.module.Initializable;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class AbstractView implements Initializable {
    protected final String id;
    protected final String imguiId;
    protected final List<AbstractView> children = new ArrayList<>();

    @PInject
    public PInjector injector;

    public AbstractView() {
        this.id = UUID.randomUUID().toString().replaceAll("-", "");
        this.imguiId = "##" + id;
    }

    public <T extends AbstractView> T appendChild(T child) {
        injector.inject(child);
        children.add(child);
        child.onInitialize();
        return child;
    }

    public void render() {
        for (AbstractView child : children) {
            child.render();
        }
    }

    @Override
    public void onInitialize() {
    }
}

