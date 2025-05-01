#ifndef VOXELPROCESSORSERVICE_H
#define VOXELPROCESSORSERVICE_H
#include "../abstract/AbstractResourceService.h"

namespace Metal {
    class VoxelProcessorService final : public AbstractResourceService {
    public:
        explicit VoxelProcessorService(ApplicationContext &context)
            : AbstractResourceService(context) {
        }

        void process();
    };
} // Metal

#endif //VOXELPROCESSORSERVICE_H
