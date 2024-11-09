package com.pine.engine.inspection;

import com.pine.engine.service.resource.LocalResourceType;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.RUNTIME)
public @interface ResourceTypeField {
    LocalResourceType type();
}
