#include "BooleanField.h"

#include <imgui.h>

#include "../../../../../common/inspection/Inspectable.h"

namespace Metal {
    BooleanField::BooleanField(InspectedField<bool> &field) : field(field) {
    }

    void BooleanField::onSync() {
        if (!field.disabled) {
            if(ImGui::Checkbox(field.id.c_str(), field.field)){
                field.instance->registerChange();
                field.instance->onUpdate(&field, *context);
            }
            ImGui::SameLine();
            ImGui::TextWrapped(field.name.c_str());
        } else {
            ImGui::Text("%s: %b", field.name.c_str(), *field.field);
        }
    }
}
