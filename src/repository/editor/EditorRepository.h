#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H

#include "../../definitions.h"
#include "../../common/inspection/Inspectable.h"
#include "../../service/audio/AudioData.h"

#define ACTUAL_WINDOW_SIZE static_cast<int>(1u << (windowSizeScale + 9))
#define ACTUAL_INTERPOLATION static_cast<int>(1u << (interpolationScale))

namespace Metal {
    struct EditorRepository final : Inspectable {
        // DEBUG
        bool showRaySearchCountVoxels = false;
        bool showRayTestCountVoxels = false;
        int voxelSearchCount = 32;
        // DEBUG

        AudioDataVector audioData;

        // PROCESSING
        int windowSizeScale = 4; // power of 2
        int actualWindowSize = ACTUAL_WINDOW_SIZE; // power of 2
        int interpolationScale = 8;
        int interpolation = ACTUAL_INTERPOLATION;
        int representationResolution = 2;
        int hopSizeScale = 2;
        float minMagnitude = 0.6;
        int maxFrequency = SAMPLE_SIZE_SECONDS;
        int maxMagnitude = SAMPLE_SIZE_SECONDS;
        float rangeStart = 0.f;
        float rangeEnd = SAMPLE_SIZE_SECONDS/2.f;
        // PROCESSING

        bool filterWindows = false;
        bool needsDataRefresh;
        int windowIndex = 0;
        bool isOrthographic = true;

        // METADATA
        std::string pathToAudio;
        float selectedAudioSize = SAMPLE_SIZE_SECONDS;
        int sampleRate;
        int channels;
        int frames;
        int actualHopSize = actualWindowSize / hopSizeScale;
        // METADATA

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;

        void onUpdate(InspectableMember *member, ApplicationContext &context) override;
    };
} // Metal

#endif
