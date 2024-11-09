package com.pine.engine.repository.core;

import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.service.resource.ResourceService;
import com.pine.engine.service.resource.shader.GLSLType;
import com.pine.engine.service.resource.ssbo.SSBOCreationData;
import com.pine.engine.service.resource.ssbo.ShaderStorageBufferObject;
import org.lwjgl.system.MemoryUtil;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import static com.pine.engine.Engine.MAX_ENTITIES;
import static com.pine.engine.Engine.MAX_LIGHTS;

@PBean
public class CoreSSBORepository implements CoreRepository {
    public ShaderStorageBufferObject octreeSSBO;
    @Override
    public void initialize() {
    }
}
