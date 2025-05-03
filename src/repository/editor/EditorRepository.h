#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H

#include "../../definitions.h"
#include "../../common/inspection/Inspectable.h"

#define ACTUAL_WINDOW_SIZE static_cast<int>(1u << (windowSizeScale + 9))
namespace Metal {
    struct EditorRepository final : Inspectable {
        bool showRaySearchCountVoxels = false;
        bool showRayTestCountVoxels = false;
        int voxelSearchCount = 32;
        int sampleSize = DEFAULT_WORLD_SIZE ;
        int windowSizeScale = 4; // power of 2
        int actualWindowSize = ACTUAL_WINDOW_SIZE; // power of 2
        int interpolation = 64;
        int hopSizeScale = 2;
        float minMagnitude = 0.6;
        int maxFrequency = 10;
        int maxMagnitude = 10;
        bool isOrthographic = false;

        std::string pathToAudio;
        float selectedAudioSize = 0.f;
        float rangeStart = 0.f;
        float rangeEnd = 10.f;
        int sampleRate;
        int channels;
        int64_t frames;
        int actualHopSize = actualWindowSize / hopSizeScale;

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;

        void onUpdate(InspectableMember *member, ApplicationContext &context) override;
    };
} // Metal

#endif
