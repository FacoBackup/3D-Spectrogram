#ifndef GLOBALREPOSITORY_H
#define GLOBALREPOSITORY_H
#include "../../common/inspection/Inspectable.h"
#include "../../service/voxel/curve/ConicalHelixCurve.h"
#include "../../service/voxel/curve/HelixCurve.h"
#include "../../definitions.h"
#include "../../service/voxel/curve/HelixVerticalCurve.h"
#include "../../service/voxel/curve/SineCurve.h"
#include "../../service/voxel/curve/SphericalSpiralCurve.h"
#include "../../service/voxel/curve/TorusKnotCurve.h"
#include "../../service/voxel/curve/VivianiCurve.h"

namespace Metal {
    struct GlobalRepository final : Inspectable {
        std::array<std::unique_ptr<AbstractCurve>, 7> curves = {
            std::make_unique<SineCurve>(),
            std::make_unique<HelixCurve>(),
            std::make_unique<TorusKnotCurve>(),
            std::make_unique<SphericalSpiralCurve>(),
            std::make_unique<ConicalHelixCurve>(),
            std::make_unique<VivianiCurve>(),
            std::make_unique<HelixVerticalCurve>()
        };
        int selectedCurve = -1;
        bool needsDataRefresh;
        bool isOrthographic = true;
        bool isShowStaticCurve = true;
        int maxXAxis = SAMPLE_SIZE_SECONDS;
        int maxZAxis = SAMPLE_SIZE_SECONDS;
        int maxYAxis = SAMPLE_SIZE_SECONDS;
        float maxDerivative = 0;
        int voxelCount = 0;
        std::string memorySize;
        bool useNyquistForTreeDepth = true;
        int actualTreeDepth = 8;

        // DEBUG
        bool showRaySearchCountVoxels = false;
        bool showRayTestCountVoxels = false;
        int voxelSearchCount = 32;

        void registerFields() override;

        const char *getTitle() override;

        const char *getIcon() override;

        void onUpdate(InspectableMember *member, ApplicationContext &context) override;
    };
} // Metal

#endif //GLOBALREPOSITORY_H
