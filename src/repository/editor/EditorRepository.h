#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H

#include "../../definitions.h"
#include "../../common/inspection/Inspectable.h"

namespace Metal {
    struct EditorRepository final : Inspectable {
        bool showRaySearchCountVoxels = false;
        bool showRayTestCountVoxels = false;
        int voxelSearchCount = 32;
        int sampleSize = DEFAULT_WORLD_SIZE / WORLD_SIZE_SCALE;
        int worldSize = DEFAULT_WORLD_SIZE;
        int windowSize = DEFAULT_WINDOW_SIZE;
        int hopSize = DEFAULT_HOP_SIZE;


        std::string pathToAudio;
        float selectedAudioSize = 0.f;
        float rangeStart = 0.f;
        float rangeEnd = 10.f;
        int sampleRate;
        int channels;
        int64_t frames;

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;

        void onUpdate(InspectableMember *member, ApplicationContext &context) override;
    };
} // Metal

#endif
