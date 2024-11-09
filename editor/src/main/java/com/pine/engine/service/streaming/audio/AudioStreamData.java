package com.pine.engine.service.streaming.audio;

import com.pine.engine.repository.streaming.StreamableResourceType;
import com.pine.engine.service.streaming.StreamLoadData;

public class AudioStreamData implements StreamLoadData {
    @Override
    public StreamableResourceType getResourceType() {
        return StreamableResourceType.AUDIO;
    }
}
