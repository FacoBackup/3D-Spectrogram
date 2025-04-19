#ifndef PASSESSERVICE_H
#define PASSESSERVICE_H
#include <memory>
#include <vector>

#include "AbstractPass.h"
#include "../../../common/AbstractRuntimeComponent.h"



namespace Metal {
    class CommandBufferRecorder;

    class PassesService final : public AbstractRuntimeComponent {
        CommandBufferRecorder *recorder = nullptr;
        AbstractPass *voxelPass = nullptr;
        AbstractPass *gridPass = nullptr;
        AbstractPass *backgroundPass = nullptr;

    public:
        explicit PassesService(ApplicationContext &context);

        void onInitialize() override;

        void onSync() override;
    };
} // Metal

#endif //PASSESSERVICE_H
