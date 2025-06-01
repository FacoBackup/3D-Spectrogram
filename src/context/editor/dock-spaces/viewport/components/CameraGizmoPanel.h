#ifndef CAMERAGIZMOPANEL_H
#define CAMERAGIZMOPANEL_H
#include <glm/mat4x4.hpp>

#include "../../../abstract/AbstractPanel.h"
#define GIZMO_SIZE 100.0f

namespace Metal {
    class CameraGizmoPanel final : public AbstractPanel {
        bool isManipulating = false;
        bool hasCopiedView = false;
        glm::mat4 tempView{};
    public:
        void onSync() override;
    };
} // Metal

#endif //CAMERAGIZMOPANEL_H
