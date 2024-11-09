package com.pine.engine.service.system;


import com.pine.engine.Engine;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.CameraRepository;
import com.pine.engine.repository.ClockRepository;
import com.pine.engine.repository.RuntimeRepository;
import com.pine.engine.repository.core.*;
import com.pine.engine.repository.voxelization.VoxelRepository;
import com.pine.engine.service.resource.SSBOService;
import com.pine.engine.service.resource.ShaderService;
import com.pine.engine.service.resource.UBOService;
import com.pine.engine.service.resource.fbo.FrameBufferObject;
import com.pine.engine.service.streaming.mesh.MeshService;

public abstract class AbstractPass {
    @PInject
    public Engine engine;
    @PInject
    public ShaderService shaderService;
    @PInject
    public SSBOService ssboService;
    @PInject
    public MeshService meshService;
    @PInject
    public CoreShaderRepository shaderRepository;
    @PInject
    public RuntimeRepository runtimeRepository;
    @PInject
    public CoreSSBORepository ssboRepository;
    @PInject
    public CoreUBORepository uboRepository;
    @PInject
    public CoreFBORepository fboRepository;
    @PInject
    public CoreMeshRepository meshRepository;
    @PInject
    public VoxelRepository voxelRepository;

    final public void render() {
        if (!isRenderable()) {
            return;
        }
        FrameBufferObject fbo = getTargetFBO();
        if (fbo != null) {
            fbo.startMapping(shouldClearFBO());
            renderInternal();
            fbo.stop();
        } else {
            renderInternal();
        }
    }

    protected FrameBufferObject getTargetFBO() {
        return null;
    }

    protected void renderInternal() {
    }

    protected boolean isRenderable() {
        return true;
    }

    protected boolean shouldClearFBO() {
        return false;
    }

    public void onInitialize() {
    }
}
