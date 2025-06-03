#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H

#include "../../definitions.h"
#include "../../common/inspection/Inspectable.h"
#include "../../service/audio/AudioData.h"
#include "../../service/voxel/curve/ConicalHelixCurve.h"
#include "../../service/voxel/curve/HelixCurve.h"
#include "../../service/voxel/curve/LorenzAttractorCurve.h"
#include "../../service/voxel/curve/ThomasAttractorCurve.h"
#include "../../service/voxel/curve/SineCurve.h"
#include "../../service/voxel/curve/SphericalSpiralCurve.h"
#include "../../service/voxel/curve/TorusKnotCurve.h"
#include "../../service/voxel/curve/VivianiCurve.h"

#define ACTUAL_WINDOW_SIZE static_cast<int>(1u << (windowSizeScale + 9))
#define ACTUAL_INTERPOLATION static_cast<int>(1u << (interpolationScale))

namespace Metal {
    class AbstractCurve;
}

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
        int representationResolution = 5;
        float maxDerivative = 0;
        int hopSizeScale = 2;
        float minMagnitude = 0.6;
        int maxZAxis = SAMPLE_SIZE_SECONDS;
        int maxYAxis = SAMPLE_SIZE_SECONDS;
        float rangeStart = 0.f;
        float rangeEnd = SAMPLE_SIZE_SECONDS/2.f;
        // PROCESSING

        bool filterWindows = false;
        bool needsDataRefresh;
        int windowIndex = 0;
        bool isOrthographic = true;

        // METADATA
        std::string pathToAudio;
        int maxXAxis = SAMPLE_SIZE_SECONDS;
        bool isShowingOriginalWave = false;
        bool isShowStaticCurve = true;
        int sampleRate;
        int channels;
        int frames;
        int actualHopSize = actualWindowSize / hopSizeScale;
        std::array<std::unique_ptr<AbstractCurve>, 6> curves = {
            std::make_unique<SineCurve>(),
            std::make_unique<HelixCurve>(),
            std::make_unique<TorusKnotCurve>(),
            std::make_unique<SphericalSpiralCurve>(),
            std::make_unique<ConicalHelixCurve>(),
            std::make_unique<VivianiCurve>()
        };
        int selectedCurve = -1;
        bool useNyquistForSpectrogramDepth = true;
        int actualTreeDepth = 8;
        // METADATA

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;

        void onUpdate(InspectableMember *member, ApplicationContext &context) override;
    };
} // Metal

#endif
