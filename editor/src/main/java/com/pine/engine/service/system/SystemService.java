package com.pine.engine.service.system;

import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.injection.PInjector;
import com.pine.engine.service.system.impl.*;
import com.pine.engine.tasks.SyncTask;

import java.util.List;

@PBean
public class SystemService implements SyncTask {
    @PInject
    public PInjector pInjector;

    private List<AbstractPass> systems = List.of(
            new VoxelVisualizerPass(),
            new GridPass()
    );

    public List<AbstractPass> getSystems() {
        return systems;
    }

    public void setSystems(List<AbstractPass> systems) {
        for (var sys : systems) {
            if (!this.systems.contains(sys)) {
                pInjector.inject(sys);
                sys.onInitialize();
            }
        }
        this.systems = systems;
    }

    @Override
    public void sync() {
        for (var system : systems) {
            system.render();
        }
    }


    public void initialize() {
        for (var sys : systems) {
            pInjector.inject(sys);
            sys.onInitialize();
        }
    }
}