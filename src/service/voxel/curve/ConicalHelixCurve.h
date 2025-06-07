#ifndef CONICALHELIXCURVE_H
#define CONICALHELIXCURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class ConicalHelixCurve : public AbstractCurve {
        float initialRadius = 1.0f;
        float taperFactor = 0.1f;
        float pitch = 0.5f;

    public:
        void registerFields() override {
            center = {2, 2, 0};

            registerBaseFields();
            registerFloat(initialRadius, "", "Initial Radius", 0.1f);
            registerFloat(taperFactor, "", "Taper Factor", 0.01f);
            registerFloat(pitch, "", "Pitch", 0.05f);
        }

        glm::vec3 evaluate(float t) const override {
            float currentRadius = initialRadius + taperFactor * t;
            if (currentRadius < 0.0f) {
                currentRadius = 0.0f;
            }

            float x = currentRadius * std::cos(t);
            float y = currentRadius * std::sin(t);
            float z = pitch * t;
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "Conical Helix: x=(r0+k*t)cos(t), y=(r0+k*t)sin(t), z=a*t";
        }
    };
}
#endif //CONICALHELIXCURVE_H
