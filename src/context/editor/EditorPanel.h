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

    public:
        void onInitialize() override;

        void renderInspector() const;

        void renderInformation();

        void onSync() override;
    };
}


#endif
