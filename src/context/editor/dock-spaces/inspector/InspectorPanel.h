#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "../../../../enum/engine-definitions.h"
#include "../../abstract/AbstractPanel.h"

namespace Metal {
    class FormPanel;
    class Inspectable;

    class InspectorPanel final : public AbstractPanel {
        FormPanel *formPanel = nullptr;

    public:
        void onInitialize() override;

        void onSync() override;
    };
}

#endif
