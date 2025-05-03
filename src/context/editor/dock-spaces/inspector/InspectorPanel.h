#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class FormPanel;
    class Inspectable;

    class InspectorPanel final : public AbstractPanel {
        FormPanel *formPanel = nullptr;
        AbstractPanel *timeStampPickerPanel = nullptr;

        void renderFileSelection() const;

    public:
        void onInitialize() override;

        void onSync() override;
    };
}

#endif
