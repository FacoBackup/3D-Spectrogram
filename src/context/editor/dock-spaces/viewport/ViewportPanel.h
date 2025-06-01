#ifndef TCC_IESB_VIEWPORTPANEL_H
#define TCC_IESB_VIEWPORTPANEL_H

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class ViewportPanel final : public AbstractPanel {
        bool isFirstMovement = false;

        void renderSpectrogramAxisLabels() const;

        void renderOriginalWaveAxisLabels() const;

        void renderCameraModeToggle() const;

        void renderFrame() const;

        void renderStaticCurveLabels() const;

        void renderContent() const;

    public:
        void onInitialize() override;

        void onSync() override;
    };
}

#endif
