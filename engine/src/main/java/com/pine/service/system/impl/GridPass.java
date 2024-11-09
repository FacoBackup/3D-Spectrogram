package com.pine.service.system.impl;

import com.pine.EngineUtils;
import com.pine.repository.rendering.RenderingMode;
import com.pine.service.resource.fbo.FrameBufferObject;
import com.pine.service.resource.shader.GLSLType;
import com.pine.service.resource.shader.UniformDTO;
import com.pine.service.system.AbstractPass;
import org.lwjgl.opengl.GL46;


public class GridPass extends AbstractPass {

    private final float[] buffer = new float[4];

    private UniformDTO depthUniform;
    private UniformDTO settingsUniform;

    @Override
    public void onInitialize() {
        settingsUniform = shaderRepository.gridShader.addUniformDeclaration("settings", GLSLType.VEC_4);
        depthUniform = shaderRepository.gridShader.addUniformDeclaration("sceneDepth", GLSLType.SAMPLER_2_D);
    }

    @Override
    protected FrameBufferObject getTargetFBO() {
        return fboRepository.auxBuffer;
    }

    @Override
    protected void renderInternal() {
        GL46.glEnable(GL46.GL_BLEND);
        GL46.glDisable(GL46.GL_CULL_FACE);
        shaderService.bind(shaderRepository.gridShader);
        buffer[0] = 0.3f;
        buffer[1] = 1;
        buffer[2] = 100;
        buffer[3] = 1f;

        GL46.glUniform4fv(settingsUniform.getLocation(), buffer);
        EngineUtils.bindTexture2d(depthUniform.getLocation(), 0, fboRepository.gBufferDepthSampler);

        meshService.bind(meshRepository.planeMesh);
        meshService.setRenderingMode(RenderingMode.TRIANGLES);
        meshService.setInstanceCount(0);
        meshService.draw();
        GL46.glEnable(GL46.GL_CULL_FACE);
    }

    @Override
    public String getTitle() {
        return "Grid rendering";
    }
}
