package com.pine;

import com.pine.injection.EngineExternalModule;
import com.pine.injection.PBean;
import com.pine.injection.PInject;
import com.pine.messaging.Loggable;
import com.pine.repository.EngineSettingsRepository;
import com.pine.repository.RuntimeRepository;
import com.pine.repository.core.*;
import com.pine.service.module.EngineModulesService;
import com.pine.service.resource.fbo.FrameBufferObject;
import com.pine.service.system.SystemService;
import com.pine.tasks.AbstractTask;
import com.pine.tasks.SyncTask;
import org.jetbrains.annotations.NotNull;
import org.lwjgl.opengl.GL46;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

@PBean
public class Engine extends MetricCollector implements Loggable {
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
    @PInject
    public List<AbstractTask> tasks;
    private boolean ready = false;
    private String targetDirectory;

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

        tasks.forEach(AbstractTask::start);

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

        start();
        for (FrameBufferObject fbo : fboRepository.all) {
            fbo.clear();
        }
        for (var syncTask : syncTasks) {
            syncTask.sync();
        }
        end();
    }


    public String getResourceTargetDirectory() {
        return targetDirectory + File.separator + "resources" + File.separator;
    }

    @Override
    public String getTitle() {
        return "Engine total";
    }
}
