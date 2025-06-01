#ifndef HARMONICPATH_H
#define HARMONICPATH_H
#include "AbstractCurve.h"

namespace Metal {
    class HarmonicPathCurve : public AbstractCurve {
        float radius = 1.0f;
        float a = 1.0f;

    public:
        void registerFields() override {
            registerBaseFields();
            registerFloat(radius, "", "Raio", 0.01f);
            registerFloat(a, "", "Altura por volta", 0.01f);
        }

        glm::vec3 evaluate(float t) const override {
            float z = radius * cos(t) + a;
            float x = radius * sin(t) + a;
            float y = a * t;
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "x(t) = rsin(t) + a, y(t) = at, z(t) = rcos(t) + a";
        }
    };
}

#endif //HARMONICPATH_H
