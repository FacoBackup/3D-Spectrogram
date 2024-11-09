package com.pine.engine.repository.core;

import com.pine.engine.Engine;
import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.RuntimeRepository;
import com.pine.engine.service.resource.ResourceService;
import com.pine.engine.service.resource.ShaderService;
import com.pine.engine.service.resource.fbo.FBOCreationData;
import com.pine.engine.service.resource.fbo.FrameBufferObject;
import org.lwjgl.opengl.GL46;

import java.util.ArrayList;
import java.util.List;

@PBean
public class CoreFBORepository implements CoreRepository {
    @PInject
    public Engine engine;
    @PInject
    public ResourceService resources;

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
