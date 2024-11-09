package com.pine.engine.service.system.impl;

import com.pine.impl.Loggable;
import com.pine.engine.repository.rendering.RenderingMode;
import com.pine.engine.service.resource.shader.Shader;
import com.pine.engine.service.system.AbstractPass;
import org.lwjgl.opengl.GL46;

public abstract class AbstractQuadPassPass extends AbstractPass implements Loggable {

    protected abstract Shader getShader();

    protected abstract void bindUniforms();

    @Override
    final protected void renderInternal() {
        GL46.glDisable(GL46.GL_DEPTH_TEST);
        shaderService.bind(getShader());
        bindUniforms();
        meshService.bind(meshRepository.quadMesh);
        meshService.setRenderingMode(RenderingMode.TRIANGLES);
        meshService.draw();
    }
}
