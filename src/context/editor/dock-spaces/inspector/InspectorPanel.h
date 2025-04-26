#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class FormPanel;
    class Inspectable;

    class InspectorPanel final : public AbstractPanel {
        FormPanel *formPanel = nullptr;
        AbstractPanel *timeStampPickerPanel = nullptr;

    public:
        void onInitialize() override;

        void renderFileSelection();

        void beginBox(std::string name, float height);

        void endBox();

        void renderAudioInfo();

        void onSync() override;
    };
}

#endif
