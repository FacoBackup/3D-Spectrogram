#ifndef TCC_IESB_EDITORPANEL_H
#define TCC_IESB_EDITORPANEL_H

#include "imgui.h"
#include "abstract/AbstractPanel.h"

namespace Metal {
    class EditorPanel final : public AbstractPanel {
        AbstractPanel *pViewport = nullptr;
        AbstractPanel *pInspector = nullptr;
        AbstractPanel *pNotifications = nullptr;
        AbstractPanel *pInformation = nullptr;

        bool isFirst = true;
        bool isInfoCollapsed = false;
        float inspectorHeight = 0;

        void renderInspector();

        void renderInformation();

    public:
        void onInitialize() override;

        void onSync() override;
    };
}


#endif
