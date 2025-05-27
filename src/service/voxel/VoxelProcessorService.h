#ifndef VOXELPROCESSORSERVICE_H
#define VOXELPROCESSORSERVICE_H
#include "../abstract/AbstractResourceService.h"
#include "../audio/AudioData.h"
#include "impl/SparseVoxelOctreeBuilder.h"

namespace Metal {
    class VoxelProcessorService final : public AbstractResourceService {
    public:
        explicit VoxelProcessorService(ApplicationContext &context)
            : AbstractResourceService(context) {
        }

        void processOriginalWave(SparseVoxelOctreeBuilder &builder);

        void processTransformedSignal(float fScale, SparseVoxelOctreeBuilder &builder);

        void process();
    };
} // Metal

#endif //VOXELPROCESSORSERVICE_H
