#include "TransformComponent.h"
#include "../../../common/interface/Icons.h"
#include "../../../context/ApplicationContext.h"

#define ROTATION "Rotation"

namespace Metal {
    void TransformComponent::registerFields() {
        registerVec3(translation, "", "Translation");
        registerVec3(scale, "", "Scale", 1);
        registerVec3(rotationEuler, "", ROTATION);
        registerBool(isStatic, "", "Static?");
    }

    ComponentTypes::ComponentType TransformComponent::getType() {
        return ComponentTypes::TRANSFORM;
    }

    void TransformComponent::onUpdate(InspectableMember *member, ApplicationContext &context) {
    }
}
