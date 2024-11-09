package com.pine.core;

import com.pine.core.dock.DockPanel;
import com.pine.core.view.AbstractView;
import imgui.ImVec4;

public abstract class AbstractWindow extends AbstractView {
    public abstract ImVec4 getNeutralPalette();

    public abstract ImVec4 getAccentColor();

    @Override
    final public String getTitle() {
        return "Total frame cost";
    }
}
