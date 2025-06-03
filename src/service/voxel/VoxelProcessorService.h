#ifndef VOXELPROCESSORSERVICE_H
#define VOXELPROCESSORSERVICE_H
#include "../abstract/AbstractResourceService.h"
#include "../audio/AudioData.h"
#include "curve/AbstractCurve.h"
#include "impl/SparseVoxelOctreeBuilder.h"

namespace Metal {
    class VoxelProcessorService final : public AbstractResourceService {
    public:
        explicit VoxelProcessorService(ApplicationContext &context)
            : AbstractResourceService(context) {
        }

        void processOriginalWave(SparseVoxelOctreeBuilder &builder);

        void processSpectrogram(float fScale, SparseVoxelOctreeBuilder &builder);

        void processAudioInfo(float fScale, SparseVoxelOctreeBuilder &builder);

        int estimateMinimumDepth(AbstractCurve &curve, float lengthScale = 1.0f) const;

        void refreshData() const;

        void process();
    };
} // Metal

#endif //VOXELPROCESSORSERVICE_H
