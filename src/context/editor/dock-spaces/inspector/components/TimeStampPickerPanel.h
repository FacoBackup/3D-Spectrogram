#ifndef AUDIOPICKERPANEL_H
#define AUDIOPICKERPANEL_H

#include "../../../abstract/AbstractPanel.h"
#include "imgui.h"

namespace Metal {
    class TimeStampPickerPanel final : public AbstractPanel {
        static void renderBackground(float timelineWidth, ImDrawList *&drawList, ImVec2 &canvasPos, ImVec2 &canvasSize,
                                     ImVec2 &canvasEnd);

        void handleDrag(float timelineWidth) const;

    public:
        void onSync() override;
    };
} // Metal

#endif //AUDIOPICKERPANEL_H
