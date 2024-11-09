package com.pine.engine.service.resource;

import com.pine.impl.Loggable;
import com.pine.engine.injection.Disposable;

public interface IResource extends Loggable, Disposable {
    String getId();

    LocalResourceType getResourceType();

    boolean isStatic();

    void makeStatic();
}
