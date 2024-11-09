package com.pine.engine.service.camera;

import com.pine.engine.EngineUtils;
import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.CameraRepository;
import com.pine.engine.repository.RuntimeRepository;
import com.pine.engine.repository.core.CoreUBORepository;
import com.pine.engine.service.resource.UBOService;
import com.pine.engine.tasks.SyncTask;

@PBean
public class CameraSyncService implements SyncTask {
    private static final double LOG_2 = Math.log(2);

    @PInject
    public CameraRepository repository;

    @PInject
    public CoreUBORepository uboRepository;

    @PInject
    public RuntimeRepository runtimeRepository;

    @PInject
    public CameraThirdPersonService cameraThirdPersonService;
    @PInject
    public UBOService uboService;

    private Camera camera;

    @Override
    public void sync() {
        camera = repository.currentCamera;
        if (camera == null) {
            return;
        }

        camera.aspectRatio = runtimeRepository.viewportW / runtimeRepository.viewportH;
        updateMatrices();
        updateUBOBuffer();
        uboService.updateBuffer(uboRepository.cameraViewUBO, uboRepository.cameraViewUBOState, 0);
    }

    private void updateMatrices() {
        updateProjection();
        updateView();
        repository.viewProjectionMatrix.set(repository.projectionMatrix).mul(repository.viewMatrix);
    }

    private void updateView() {
        cameraThirdPersonService.createViewMatrix(camera);
        repository.viewMatrix.invert(repository.invViewMatrix);
        repository.staticViewMatrix.set(repository.viewMatrix);
        repository.staticViewMatrix.m30(0).m31(0).m32(0);
    }

    private void updateProjection() {
        camera.aspectRatio = runtimeRepository.viewportW / runtimeRepository.viewportH;
        if (camera.isOrthographic) {
            repository.projectionMatrix.setOrtho(-camera.orthographicProjectionSize, camera.orthographicProjectionSize,
                    -camera.orthographicProjectionSize / camera.aspectRatio, camera.orthographicProjectionSize / camera.aspectRatio,
                    -camera.zFar, camera.zFar);
        } else {
            repository.projectionMatrix.setPerspective(camera.fov, camera.aspectRatio, camera.zNear, camera.zFar);
        }
        repository.skyboxProjectionMatrix.setPerspective(camera.fov, camera.aspectRatio, 0.1f, 1000f);
        repository.invSkyboxProjectionMatrix.set(repository.skyboxProjectionMatrix).invert();
        repository.invProjectionMatrix.set(repository.projectionMatrix).invert();
    }

    private void updateUBOBuffer() {
        var V = uboRepository.cameraViewUBOState;
        EngineUtils.copyWithOffset(V, repository.viewProjectionMatrix, 0);
        EngineUtils.copyWithOffset(V, repository.viewMatrix, 16);
        EngineUtils.copyWithOffset(V, repository.invViewMatrix, 32);
        EngineUtils.copyWithOffset(V, camera.position, 48);
        EngineUtils.copyWithOffset(V, repository.projectionMatrix, 52);
        EngineUtils.copyWithOffset(V, repository.invProjectionMatrix, 68);

        V.put(84, runtimeRepository.getDisplayW());
        V.put(85, runtimeRepository.getDisplayH());
        V.put(86, (float) (2.0 / (Math.log(repository.projectionMatrix.get(0, 0) + 1) / LOG_2)));
    }
}