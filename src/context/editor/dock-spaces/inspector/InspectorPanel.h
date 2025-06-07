#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class FormPanel;
    class Inspectable;

    class InspectorPanel final : public AbstractPanel {
        FormPanel *formPanel = nullptr;
        FormPanel *formPanel2 = nullptr;
        AbstractPanel *timeStampPickerPanel = nullptr;

        void renderFileSelection() const;

        void renderStaticCurveSettings();

    public:
        void onInitialize() override;

        void updateAudioRelatedProperties() const;

        void onSync() override;
    };
}

#endif
