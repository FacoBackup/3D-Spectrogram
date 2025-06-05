#ifndef SINECURVE_H
#define SINECURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class SineCurve : public AbstractCurve {
        float amplitude = 1.0f;
        float frequency = 1.0f;

    public:
        void registerFields() override {
            registerBaseFields();
            registerFloat(amplitude, "", "Amplitude", 0.01f);
            registerFloat(frequency, "", "Frequência", 0.01f);
        }

        glm::vec3 evaluate(float t) const override {
            float x = t;
            float y = amplitude * sin(frequency * t);
            return glm::vec3(x, y, 0.0f);
        }

        std::string getCurveName() override {
            return "Seno: x(t) = t, y(t) = sin(t) * a, z(t) = 0";
        }
    };
}

#endif //SINECURVE_H
