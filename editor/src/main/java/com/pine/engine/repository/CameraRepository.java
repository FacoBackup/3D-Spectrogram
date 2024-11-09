package com.pine.engine.repository;

import com.pine.impl.Icons;
import com.pine.engine.injection.PBean;
import com.pine.engine.inspection.Inspectable;
import com.pine.engine.inspection.MutableField;
import com.pine.engine.service.camera.Camera;
import org.joml.Matrix4f;

@PBean
public class CameraRepository extends Inspectable  {
    @MutableField(group = "Controls", label = "Camera rotation sensitivity")
    public float sensitivity = 1;

    @MutableField(group = "Controls", label = "Orbit camera zoom sensitivity")
    public float zoomSensitivity = 1.0f;

    public final Matrix4f viewMatrix = new Matrix4f();
    public final Matrix4f projectionMatrix = new Matrix4f();
    public final Matrix4f invViewMatrix = new Matrix4f();
    public final Matrix4f invProjectionMatrix = new Matrix4f();
    public final Matrix4f viewProjectionMatrix = new Matrix4f();
    public final Matrix4f staticViewMatrix = new Matrix4f();
    public final Matrix4f skyboxProjectionMatrix = new Matrix4f();
    public final Matrix4f invSkyboxProjectionMatrix = new Matrix4f();
    public float lastMouseX;
    public float lastMouseY;
    public Camera currentCamera = new Camera();
    public float deltaX;
    public float deltaY;

    /**
     * Will force camera update if the instance is different from the current one
     *
     * @param camera
     */
    public void setCurrentCamera(Camera camera) {
        if (camera != null && camera != currentCamera) {
            currentCamera = camera;
        }
    }

    public Camera getCurrentCamera() {
        return currentCamera;
    }

    @Override
    public String getTitle() {
        return "Camera & lenses";
    }

    @Override
    public String getIcon() {
        return Icons.camera;
    }
}
