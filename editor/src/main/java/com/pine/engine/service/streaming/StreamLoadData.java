package com.pine.engine.service.streaming;

import com.pine.engine.repository.streaming.StreamableResourceType;

import java.io.Serializable;

public interface StreamLoadData extends Serializable {
    StreamableResourceType getResourceType();
}
