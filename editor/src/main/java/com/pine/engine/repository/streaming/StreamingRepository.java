package com.pine.engine.repository.streaming;

import com.pine.engine.injection.PBean;
import com.pine.engine.service.streaming.StreamLoadData;

import javax.annotation.Nullable;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

@PBean
public class StreamingRepository {
    public final List<AbstractStreamableResource<?>> streamableResources = new Vector<>();
    public transient final Map<String, AbstractStreamableResource<?>> schedule = new ConcurrentHashMap<>();
    public transient final Map<String, StreamLoadData> loadedResources = new ConcurrentHashMap<>();

    @Nullable
    public AbstractStreamableResource<?> getById(String resourceId) {
        return streamableResources.stream().filter(a -> a.id.equals(resourceId)).findFirst().orElse(null);
    }
}
