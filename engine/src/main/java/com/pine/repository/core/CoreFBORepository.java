package com.pine.repository.core;

import com.pine.Engine;
import com.pine.injection.PBean;
import com.pine.injection.PInject;
import com.pine.repository.EngineSettingsRepository;
import com.pine.repository.RuntimeRepository;
import com.pine.service.resource.ResourceService;
import com.pine.service.resource.ShaderService;
import com.pine.service.resource.fbo.FBOCreationData;
import com.pine.service.resource.fbo.FrameBufferObject;
import org.lwjgl.opengl.GL46;

import java.util.ArrayList;
import java.util.List;

@PBean
public class CoreFBORepository implements CoreRepository {
    @PInject
    public Engine engine;
    @PInject
    public ShaderService shaderService;
    @PInject
    public ResourceService resources;
    @PInject
    public EngineSettingsRepository configuration;
    @PInject
    public RuntimeRepository runtimeRepository;


    public int gBufferDepthSampler;
    public FrameBufferObject auxBuffer;
    public int auxSampler;
    public final List<FrameBufferObject> all = new ArrayList<>();

    @Override
    public void initialize() {
        auxBuffer = (FrameBufferObject) resources.addResource(new FBOCreationData(false, true)
                .addSampler(0, GL46.GL_RGBA16F, GL46.GL_RGBA, GL46.GL_FLOAT, true, false)
                .staticResource());

        auxSampler = auxBuffer.getSamplers().getFirst();

        all.add(auxBuffer);
    }
}
