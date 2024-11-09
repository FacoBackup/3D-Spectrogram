package com.pine.engine.service.system.impl;

import com.pine.engine.service.resource.fbo.FrameBufferObject;
import com.pine.engine.service.resource.shader.GLSLType;
import com.pine.engine.service.resource.shader.Shader;
import com.pine.engine.service.resource.shader.UniformDTO;
import org.lwjgl.system.MemoryUtil;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class VoxelVisualizerPass extends AbstractQuadPassPass {
    private final FloatBuffer centerScaleBuffer = MemoryUtil.memAllocFloat(4);
    private final IntBuffer settingsBuffer = MemoryUtil.memAllocInt(3);
    private UniformDTO centerScale;
    private UniformDTO settings;

    @Override
    public void onInitialize() {
        centerScale = shaderRepository.voxels.addUniformDeclaration("centerScale", GLSLType.VEC_4);
        settings = shaderRepository.voxels.addUniformDeclaration("settings", GLSLType.IVEC_3);
    }

    @Override
    protected boolean isRenderable() {
        return ssboRepository.octreeSSBO != null;
    }

    @Override
    protected FrameBufferObject getTargetFBO() {
        return fboRepository.auxBuffer;
    }

    @Override
    protected Shader getShader() {
        return shaderRepository.voxels;
    }

    @Override
    protected void bindUniforms() {
        ssboService.bind(ssboRepository.octreeSSBO);

        centerScaleBuffer.put(0, voxelRepository.center.x);
        centerScaleBuffer.put(1, voxelRepository.center.y);
        centerScaleBuffer.put(2, voxelRepository.center.z);
        centerScaleBuffer.put(3, voxelRepository.gridResolution);
        shaderService.bindUniform(centerScale, centerScaleBuffer);

        settingsBuffer.put(0, voxelRepository.randomColors ? 1 : 0);
        settingsBuffer.put(1, voxelRepository.showRaySearchCount ? 1 : 0);
        settingsBuffer.put(2, voxelRepository.showRayTestCount ? 1 : 0);
        shaderService.bindUniform(settings, settingsBuffer);
    }

}
