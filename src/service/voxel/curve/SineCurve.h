#ifndef SINECURVE_H
#define SINECURVE_H
#include "AbstractCurve.h"

namespace Metal {
    class SineCurve final : public AbstractCurve {
    public:
        std::string getName() override {
            return "Sen(X)";
        }

    protected:
        float runFunctionX(float axisData) override {
            return axisData;
        }

        float runFunctionY(float axisData) override {
            return sin(axisData);
        }

        float runFunctionZ(float axisData) override {
            return 0;
        }
    };
}

#endif //SINECURVE_H
