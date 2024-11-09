package com.pine.engine.repository.core;

import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.service.resource.ResourceService;
import com.pine.engine.service.resource.shader.Shader;
import com.pine.engine.service.resource.shader.ShaderCreationData;

import static com.pine.engine.service.resource.shader.ShaderCreationData.LOCAL_SHADER;

@PBean
public class CoreShaderRepository implements CoreRepository {
    public Shader voxels;
    public Shader gridShader;

    @PInject
    public ResourceService resources;

    @Override
    public void initialize() {
        gridShader = (Shader) resources.addResource(new ShaderCreationData(LOCAL_SHADER + "tool/GRID.vert", LOCAL_SHADER + "tool/GRID.frag"));
        voxels = (Shader) resources.addResource(new ShaderCreationData(LOCAL_SHADER + "QUAD.vert", LOCAL_SHADER + "VOXELS.frag").staticResource());
    }
}
