#include "InspectorPanel.h"

#include "../../abstract/form/FormPanel.h"
#include "../../../../util/UIUtil.h"
#include "../../../../common/inspection/Inspectable.h"
#include "../../../../context/ApplicationContext.h"

namespace Metal {
    void InspectorPanel::onInitialize() {
        formPanel = new FormPanel();
        appendChild(formPanel);
    }

    void InspectorPanel::onSync() {
        if (ImGui::BeginChild((id + "form").c_str())) {
            formPanel->setInspection(&context->editorRepository);
            formPanel->onSync();
        }
        ImGui::EndChild();
    }
}
