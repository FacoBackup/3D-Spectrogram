#ifndef TCC_IESB_VIEWPORTPANEL_H
#define TCC_IESB_VIEWPORTPANEL_H

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class ViewportPanel final : public AbstractPanel {
        bool isFirstMovement = false;

    public:
        void onSync() override;

        void renderCameraGizmo();

        void updateCamera();

        void updateInputs() const;
    };
}

#endif
