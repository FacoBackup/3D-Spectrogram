#ifndef TCC_IESB_VIEWPORTPANEL_H
#define TCC_IESB_VIEWPORTPANEL_H

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class ViewportPanel final : public AbstractPanel {
        bool isFirstMovement = false;
        AbstractPanel *cameraPanel = nullptr;

    public:
        void onInitialize() override;

        void onSync() override;

        void updateCamera();

        void updateInputs() const;
    };
}

#endif
