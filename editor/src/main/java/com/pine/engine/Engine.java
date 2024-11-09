package com.pine.engine;

import com.pine.impl.Loggable;
import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.RuntimeRepository;
import com.pine.engine.repository.core.*;
import com.pine.engine.service.resource.fbo.FrameBufferObject;
import com.pine.engine.service.system.SystemService;
import com.pine.engine.tasks.SyncTask;
import org.lwjgl.opengl.GL46;

import java.util.List;

@PBean
public class Engine implements Loggable {
    public static final int MAX_ENTITIES = 100000;
    public static final int MAX_LIGHTS = 310;

    @PInject
    public SystemService systemsService;
    @PInject
    public CoreShaderRepository shaderRepository;
    @PInject
    public CoreSSBORepository ssboRepository;
    @PInject
    public CoreUBORepository uboRepository;
    @PInject
    public CoreFBORepository fboRepository;
    @PInject
    public CoreMeshRepository primitiveRepository;
    @PInject
    public RuntimeRepository runtimeRepository;
    @PInject
    public List<SyncTask> syncTasks;
    private boolean ready = false;

    public void start(int displayW, int displayH) {
        runtimeRepository.setDisplayW(displayW);
        runtimeRepository.setDisplayH(displayH);
        runtimeRepository.setInvDisplayW(1f / displayW);
        runtimeRepository.setInvDisplayH(1f / displayH);

        setupGL();

        primitiveRepository.initialize();
        ssboRepository.initialize();
        uboRepository.initialize();
        fboRepository.initialize();
        shaderRepository.initialize();
        systemsService.initialize();

        ready = true;
    }

    private static void setupGL() {
        GL46.glEnable(GL46.GL_BLEND);
        GL46.glBlendFunc(GL46.GL_SRC_ALPHA, GL46.GL_ONE_MINUS_SRC_ALPHA);
        GL46.glEnable(GL46.GL_CULL_FACE);
        GL46.glCullFace(GL46.GL_BACK);
        GL46.glEnable(GL46.GL_DEPTH_TEST);
        GL46.glDepthFunc(GL46.GL_LESS);
        GL46.glFrontFace(GL46.GL_CCW);
    }

    public void render() {
        if (!ready) {
            return;
        }

        for (FrameBufferObject fbo : fboRepository.all) {
            fbo.clear();
        }
        for (var syncTask : syncTasks) {
            syncTask.sync();
        }
    }


    public String getResourceTargetDirectory() {
        return null;
    }

}
