#ifndef SPHERICALSPIRALCURVE_H
#define SPHERICALSPIRALCURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class SphericalSpiralCurve : public AbstractCurve {
        float sphereRadius = 1.0f;
        float k_factor = 5.0f;

    public:
        void registerFields() override {
            center = {1, 2, 2};

            registerBaseFields();
            registerFloat(sphereRadius, "", "Sphere Radius (R)", 0.1f);
            registerFloat(k_factor, "", "Spiral Factor (k)", 0.1f);
        }

        glm::vec3 evaluate(float t) const override {
            float x = sphereRadius * std::cos(k_factor * t) * std::sin(t);
            float y = sphereRadius * std::sin(k_factor * t) * std::sin(t);
            float z = sphereRadius * std::cos(t);
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "Spherical Spiral: x(t) = rcos(kt)sin(t), y(t) = rsin(kt)sin(t), z(t) = rcos(t)";
        }
    };
}

#endif //SPHERICALSPIRALCURVE_H
