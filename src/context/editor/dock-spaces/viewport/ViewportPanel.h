#ifndef TCC_IESB_VIEWPORTPANEL_H
#define TCC_IESB_VIEWPORTPANEL_H

#include <glm/mat4x4.hpp>

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class ViewportPanel final : public AbstractPanel {
        bool isFirstMovement = false;
        bool isManipulating = false;
        bool hasCopiedView = false;
        glm::mat4 tempView{};

    public:
        void renderContent();

        void onSync() override;

        void renderCameraGizmo();

        void updateCamera();

        void updateInputs() const;
    };
}

#endif
