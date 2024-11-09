package com.pine.engine.service.camera;


import org.joml.Vector3f;

import java.io.Serializable;
import java.util.UUID;

public class Camera implements Serializable {
    public final String id = UUID.randomUUID().toString();
    public final Vector3f position = new Vector3f();
    public final Vector3f orbitCenter = new Vector3f();
    public float pitch = 0f;
    public float yaw = 0f;
    public float orbitRadius = 1;
    public boolean orbitalMode = false;
    public boolean isOrthographic = false;
    public float zFar = 10000;
    public float zNear = .1f;
    public float fov = (float) Math.toRadians(60);
    public float aspectRatio = 1;
    public float orthographicProjectionSize = 50;
}

