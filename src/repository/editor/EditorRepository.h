#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H
#include <imgui.h>
#include <ImGuizmo.h>
#include <sndfile.h>
#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>

#include "../../common/inspection/Inspectable.h"

namespace Metal {
    struct EditorRepository final : Inspectable {
        bool showRaySearchCountVoxels = false;
        bool showRayTestCountVoxels = false;
        int voxelSearchCount = 32;

        std::string pathToAudio;
        float selectedAudioSize = 0.f;
        float rangeStart = 1.0f;
        float rangeEnd = 3.5f;
        int sampleRate;
        int channels;
        int64_t frames;

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;
    };
} // Metal

#endif
