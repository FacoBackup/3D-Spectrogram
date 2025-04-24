#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H
#include <imgui.h>
#include <ImGuizmo.h>
#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>

#include "../../common/inspection/Inspectable.h"

namespace Metal {
    struct AudioMetaData {
    };

    struct EditorRepository final : Inspectable {
        bool showRaySearchCountVoxels = false;
        bool showRayTestCountVoxels = false;
        int voxelSearchCount = 32;

        ImVec4 accent{};
        ImU32 accentU32 = 0;

        glm::vec3 accentColor{0.26f, 0.59f, 0.98f};
        glm::vec3 backgroundColor{.8};
        bool isDarkMode = true;
        std::string pathToAudio;
        float selectedAudioSize = 0.f;
        float rangeStart = 1.0f;
        float rangeEnd = 3.5f;

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;
    };
} // Metal

#endif
