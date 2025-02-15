#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H
#include <imgui.h>
#include <ImGuizmo.h>
#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>

#include "../../util/serialization-definitions.h"
#include "../../enum/engine-definitions.h"
#include "../../enum/EditorMode.h"
#include "../../enum/ShadingMode.h"
#include "../../common/inspection/Inspectable.h"

namespace Metal {

    struct EditorRepository final : Inspectable {
        bool showRaySearchCountVoxels = true;
        bool showRayTestCountVoxels = true;
        int voxelSearchCount = 32;

        ImVec4 accent{};
        ImU32 accentU32 = 0;

        glm::vec3 accentColor{0.26f, 0.59f, 0.98f};
        bool isDarkMode = true;
        bool showGrid = true;
        float gridScale = 1.f;
        int gridThreshold = 100;
        float gridThickness = .1;


        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;
    };
} // Metal

#endif
