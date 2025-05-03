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

        void Draw3DLabel(const glm::vec3 &worldPos, const char *text, const glm::mat4 &view, const glm::mat4 &proj,
                         int screenWidth, int screenHeight);

        void renderContent();

        void renderCameraGizmo();

    public:
        void onSync() override;
    };
}

#endif
