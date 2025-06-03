#ifndef THOMASATTRACTOR_H
#define THOMASATTRACTOR_H
#include "AbstractCurve.h"

namespace Metal {
    class ThomasAttractorCurve : public AbstractCurve {
        float b = 0.208f;

    public:
        void registerFields() override {
            registerBaseFields();
            registerFloat(b, "", "b", 0.001f);
        }

        std::string getCurveName() override {
            return "x'=sin(y)−bx, y'=sin(z)−by, z'=sin(x)−bz";
        }

        MaxAxis addVoxels(SparseVoxelOctreeBuilder &builder) override {
            MaxAxis maxAxis{};
            glm::vec3 point(1.0f, 1.0f, 1.0f);
            float dt = getIteration();

            for (float t = 0.0f; t <= getMaxT(); t += dt) {
                float dx = sin(point.y) - b * point.x;
                float dy = sin(point.z) - b * point.y;
                float dz = sin(point.x) - b * point.z;

                point += glm::vec3(dx, dy, dz) * dt;
                builder.insert(point);

                maxAxis.x = std::max(maxAxis.x, point.x);
                maxAxis.y = std::max(maxAxis.y, point.y);
                maxAxis.z = std::max(maxAxis.z, point.z);
            }

            return maxAxis;
        }
    };
}
#endif //THOMASATTRACTOR_H
