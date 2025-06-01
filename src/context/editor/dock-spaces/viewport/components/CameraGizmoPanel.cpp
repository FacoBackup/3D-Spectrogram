#include "CameraGizmoPanel.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>
#include <sndfile.h>
#include <vector>
#include <glm/vec3.hpp>
#include "../../../../../context/ApplicationContext.h"

namespace Metal {
    void CameraGizmoPanel::onSync() {
        auto &camera = context->engineContext.camera;

        ImGuizmo::SetDrawlist();

        ImVec2 viewportPos = ImGui::GetWindowPos();
        ImVec2 viewportSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

        ImVec2 gizmoPos = ImVec2(viewportPos.x + viewportSize.x - GIZMO_SIZE, viewportPos.y);

        if (tempView != camera.viewMatrix && !isManipulating) {
            tempView = camera.viewMatrix;
        }
        ImGuizmo::ViewManipulate(glm::value_ptr(tempView), 4.0f, gizmoPos, ImVec2(GIZMO_SIZE, GIZMO_SIZE),
                                 IM_COL32(40, 40, 40, 0));
        ImGui::SetCursorPos(gizmoPos);
        ImGui::InvisibleButton("gizmoButton", ImVec2(GIZMO_SIZE, GIZMO_SIZE));

        isManipulating = camera.viewMatrix != tempView && ImGui::IsItemHovered();
        if (isManipulating) {
            glm::vec3 forward = glm::normalize(glm::inverse(tempView)[2]); // world-space forward

            camera.yaw = atan2(forward.x, forward.z);
            camera.pitch = asin(forward.y);

            camera.changed = true;
        }
    }
} // Metal