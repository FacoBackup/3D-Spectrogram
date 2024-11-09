package com.pine.impl;

import com.pine.engine.Engine;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.CameraRepository;
import com.pine.engine.repository.RuntimeRepository;
import com.pine.engine.service.camera.Camera;
import com.pine.engine.service.camera.CameraThirdPersonService;
import imgui.ImGui;
import imgui.ImGuiIO;
import imgui.ImVec2;
import imgui.ImVec4;
import imgui.extension.imguizmo.ImGuizmo;
import imgui.flag.ImGuiKey;
import imgui.flag.ImGuiMouseButton;
import org.joml.Vector3f;

public class ViewportPanel extends AbstractView {
    private static final ImVec4 RED = new ImVec4(1, 0, 0, 1);
    private static final ImVec4 GREEN = new ImVec4(0, 1, 0, 1);
    private static final ImVec4 BLUE = new ImVec4(0, .5f, 1, 1);

    @PInject
    public Engine engine;

    @PInject
    public RuntimeRepository repo;

    @PInject
    public CameraRepository cameraRepository;

    @PInject
    public CameraThirdPersonService cameraThirdPersonService;

    private final ImVec2 sizeVec = new ImVec2();
    private final ImVec2 INV_X = new ImVec2(1, 0);
    private final ImVec2 INV_Y = new ImVec2(0, 1);
    public final Camera camera = new Camera();

    private ImGuiIO io;
    private boolean isFirstMovement;

    @Override
    public void onInitialize() {
        camera.pitch = (float) -(Math.PI / 4);
        camera.yaw = (float) (Math.PI / 4);
        camera.orbitRadius = 10;
        camera.orbitalMode = true;

        io = ImGui.getIO();
    }

    @Override
    public void render() {

        tick();

        Vector3f positionCamera = cameraRepository.currentCamera.position;
        ImGui.textColored(RED, "X: " + positionCamera.x);
        ImGui.sameLine();
        ImGui.textColored(GREEN, "Y: " + positionCamera.y);
        ImGui.sameLine();
        ImGui.textColored(BLUE, "Z: " + positionCamera.z);
        ImGui.sameLine();
        ImGui.textDisabled(" | " + io.getFramerate() + " fps");
        ImGui.image(engine.fboRepository.auxSampler, new ImVec2(ImGui.getWindowSize().x, ImGui.getWindowSize().y - 50), INV_Y, INV_X);
    }

    private void tick() {
        cameraRepository.setCurrentCamera(camera);

        boolean focused = ImGui.isWindowFocused() && !ImGuizmo.isUsing();
        updateCamera(focused);
        engine.render();

        sizeVec.x = ImGui.getWindowSizeX();
        sizeVec.y = ImGui.getWindowSizeY();
    }

    private void updateCamera(boolean focused) {
            if (focused) {
                if (io.getMouseWheel() != 0 && ImGui.isWindowHovered()) {
                    cameraThirdPersonService.zoom(camera, io.getMouseWheel());
                }
                if (io.getMouseDown(ImGuiMouseButton.Left) && io.getMouseDown(ImGuiMouseButton.Right)) {
                    cameraThirdPersonService.isChangingCenter = true;
                    cameraThirdPersonService.changeCenter(camera, isFirstMovement);
                } else {
                    cameraThirdPersonService.isChangingCenter = false;
                }
            }

        if (focused && (ImGui.isMouseDown(ImGuiMouseButton.Left) || ImGui.isMouseDown(ImGuiMouseButton.Right) || (ImGui.isMouseDown(ImGuiMouseButton.Middle) && camera.orbitalMode))) {
            cameraThirdPersonService.handleInput(camera, isFirstMovement);
            isFirstMovement = false;
        } else {
            isFirstMovement = true;
        }
        repo.fasterPressed = ImGui.isKeyDown(ImGuiKey.LeftShift);
        repo.forwardPressed = ImGui.isKeyDown(ImGuiKey.W);
        repo.backwardPressed = ImGui.isKeyDown(ImGuiKey.S);
        repo.leftPressed = ImGui.isKeyDown(ImGuiKey.A);
        repo.rightPressed = ImGui.isKeyDown(ImGuiKey.D);
        repo.upPressed = ImGui.isKeyDown(ImGuiKey.Space);
        repo.downPressed = ImGui.isKeyDown(ImGuiKey.LeftCtrl);
        repo.mouseX = ImGui.getMousePosX();
        repo.mouseY = ImGui.getMousePosY();
        repo.viewportH = sizeVec.y;
        repo.viewportW = sizeVec.x;
    }

}
