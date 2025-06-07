#ifndef VIVIANICURVE_H
#define VIVIANICURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class VivianiCurve : public AbstractCurve {
        float radius_a = 1.0f;

    public:
        void registerFields() override {
            registerBaseFields();
            center = {1, 2, 2};
            maxT = 15;

            registerFloat(radius_a, "", "Radius (a)", 0.1f);
        }

        glm::vec3 evaluate(float t) const override {
            // t ranges from 0 to 4*PI for the full curve.
            float x = radius_a * (1.0f + std::cos(t));
            float y = radius_a * std::sin(t);
            float z = 2.0f * radius_a * std::sin(t / 2.0f);
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "Viviani's Curve: x=a(1+cos(t)), y=asin(t), z=2asin(t/2)";
        }
    };
}

#endif //VIVIANICURVE_H
