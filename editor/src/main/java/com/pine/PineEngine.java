package com.pine;

import com.pine.engine.injection.PInjector;

public class PineEngine {
    private static final PInjector INJECTOR = new PInjector(PineEngine.class.getPackageName());

    public static void main(String[] args) {
        INJECTOR.boot();
        System.exit(0);
    }
}
