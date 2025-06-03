#ifndef TORUSKNOTCURVE_H
#define TORUSKNOTCURVE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "AbstractCurve.h"

namespace Metal {
    class TorusKnotCurve : public AbstractCurve {
        float R_major = 2.0f;
        float r_minor = 0.5f;
        int p_windings = 2;
        int q_windings = 3;

    public:
        void registerFields() override {
            registerBaseFields();
            registerFloat(R_major, "", "Major Radius (R)", 0.1f);
            registerFloat(r_minor, "", "Minor Radius (r)", 0.05f);
            registerInt(p_windings, "", "P Windings", 1);
            registerInt(q_windings, "", "Q Windings", 1);
        }

        glm::vec3 evaluate(float t) const override {
            float p_t = static_cast<float>(p_windings) * t;
            float q_t = static_cast<float>(q_windings) * t;

            float common_factor = R_major + r_minor * std::cos(q_t);
            float x = common_factor * std::cos(p_t);
            float y = common_factor * std::sin(p_t);
            float z = r_minor * std::sin(q_t);
            return glm::vec3(x, y, z);
        }

        std::string getCurveName() override {
            return "Torus Knot: x=(R+r*cos(q*t))cos(p*t), y=(R+r*cos(q*t))sin(p*t), z=r*sin(q*t)";
        }
    };
}
#endif //TORUSKNOTCURVE_H
