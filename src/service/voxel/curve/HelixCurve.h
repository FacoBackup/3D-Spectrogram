#ifndef HELIXCURVE_H
#define HELIXCURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class HelixCurve : public AbstractCurve {
        float radius = 1.0f;
        float a = 1.0f;

    public:
        void registerFields() override {
            center = {2, 2, 0};

            registerBaseFields();
            registerFloat(radius, "", "Raio", 0.01f);
            registerFloat(a, "", "Altura por volta", 0.01f);
        }

        glm::vec3 evaluate(float t) const override {
            float x = radius * cos(t);
            float y = radius * sin(t);
            float z = a * t;
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "Helix: x(t) = rcos(t), y(t) = rsin(t), z(t) = at";
        }
    };
}
#endif //HELIXCURVE_H
