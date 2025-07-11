#include "Inspectable.h"

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "InspectedField.h"
#include "InspectedMethod.h"
#include "../../util/Util.h"

#define  DECLARATION(T, V) \
        std::shared_ptr<InspectedField<T>> field = std::make_shared<InspectedField<T>>(&v);\
        field->name = std::move(name);\
        field->id = "##" + Util::uuidV4();\
        field->nameWithId = field->name + field->id;\
        field->group = std::move(group);\
        field->disabled = disabled;\
        field->type = V;\
        field->instance = this;


#define  NUMERIC_DECLARATION \
        field->minF = min;\
        field->maxF = max;\
        field->incrementF = increment;\
        fields.push_back(std::move(field));

namespace Metal {
    void Inspectable::registerMethod(const std::function<void()> &updateCallback, std::string name, std::string group) {
        auto field = std::make_unique<InspectedMethod>();
        field->group = std::move(group);
        field->name = std::move(name);
        field->instance = this;
        field->nameWithId = field->name + Util::uuidV4();


        field->callback = updateCallback;
        field->type = FieldType::METHOD;
        fields.push_back(std::move(field));
    }

    std::vector<std::shared_ptr<InspectableMember> > &Inspectable::getFields() {
        if (!fieldsRegistered) {
            fieldsRegistered = true;
            registerFields();
        }
        return fields;
    }

    unsigned long Inspectable::getChangeId() const {
        return changes;
    }

    void Inspectable::registerChange() {
        changes++;
    }

    bool Inspectable::isNotFrozen() const {
        return frozenVersion != getChangeId();
    }

    void Inspectable::freezeVersion() {
        frozenVersion = getChangeId();
    }

    void Inspectable::registerInt(int &v,
                                  std::string group, std::string name,
                                  int min, int max, bool disabled) {
        DECLARATION(int, INT)
        field->min = min;
        field->max = max;
        fields.push_back(std::move(field));
    }

    void Inspectable::registerText(std::string &v,
                                   std::string group, std::string name, bool disabled) {
        DECLARATION(std::string, STRING);
        fields.push_back(std::move(field));
    }

    void Inspectable::registerFloat(float &v,
                                    std::string group, std::string name,
                                    float min, float max, bool disabled, float increment) {
        DECLARATION(float, FLOAT)
        NUMERIC_DECLARATION
    }

    void Inspectable::registerVec2(glm::vec2 &v,
                                   std::string group, std::string name,
                                   float min, float max, bool disabled, float increment) {
        DECLARATION(glm::vec2, VECTOR2)
        NUMERIC_DECLARATION
    }

    void Inspectable::registerVec3(glm::vec3 &v,
                                   std::string group, std::string name,
                                   float min, float max, bool disabled, float increment) {
        DECLARATION(glm::vec3, VECTOR3)
        NUMERIC_DECLARATION
    }

    void Inspectable::registerVec4(glm::vec4 &v,
                                   std::string group, std::string name,
                                   float min, float max, bool disabled, float increment) {
        DECLARATION(glm::vec4, VECTOR4)
        NUMERIC_DECLARATION
    }

    void Inspectable::registerQuat(glm::quat &v,
                                   std::string group, std::string name,
                                   float min, float max, bool disabled, float increment) {
        DECLARATION(glm::quat, QUAT)
        NUMERIC_DECLARATION
    }

    void Inspectable::registerColor(glm::vec3 &v,
                                    std::string group, std::string name, bool disabled) {
        DECLARATION(glm::vec3, COLOR)
        fields.push_back(std::move(field));
    }

    void Inspectable::registerBool(bool &v,
                                   std::string group, std::string name, bool disabled) {
        DECLARATION(bool, BOOLEAN)
        fields.push_back(std::move(field));
    }
}
