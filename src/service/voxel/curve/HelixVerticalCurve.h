#ifndef HELIXVERTICALCURVE_H
#define HELIXVERTICALCURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class HelixVerticalCurve : public AbstractCurve {
        float radius = 1.0f;
        float a = 1.0f;

    public:
        void registerFields() override {
            registerBaseFields();
            registerFloat(radius, "", "Raio", 0.01f);
            registerFloat(a, "", "Altura por volta", 0.01f);
        }

        glm::vec3 evaluate(float t) const override {
            float x = radius * cos(t);
            float z = radius * sin(t);
            float y = a * t;
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "Helix vertical: x(t) = rcos(t), y(t) = at, z(t) = rsin(t)";
        }
    };
}
#endif //HELIXCURVE_H
