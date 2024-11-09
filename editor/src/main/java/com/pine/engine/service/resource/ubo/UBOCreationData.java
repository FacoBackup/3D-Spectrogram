package com.pine.engine.service.resource.ubo;

import com.pine.engine.service.resource.LocalResourceType;
import com.pine.engine.service.resource.ResourceCreationData;

import java.util.List;

public class UBOCreationData extends ResourceCreationData {
    private final List<UBOData> data;
    private final String blockName;

    public UBOCreationData(String blockName, UBOData ...data) {
        this.data = List.of(data);
        this.blockName = blockName;
    }

    @Override
    public LocalResourceType getResourceType() {
        return LocalResourceType.UBO;
    }

    public List<UBOData> data() {
        return data;
    }

    public String blockName() {
        return blockName;
    }
}
