#ifndef CONTROLSERVICE_H
#define CONTROLSERVICE_H
#include "../../common/interface/Synchornizable.h"
#include "../abstract/AbstractResourceService.h"

namespace Metal {
    class ControlService final : AbstractResourceService, Syncronizable {
        bool isFirstMovement = true;

        void updateCamera();

        void updateInputs() const;

    public:
        explicit ControlService(ApplicationContext &context)
            : AbstractResourceService(context) {
        }

        void onSync() override;
    };
} // Metal

#endif //CONTROLSERVICE_H
