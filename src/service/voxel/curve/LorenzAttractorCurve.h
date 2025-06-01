#ifndef LORENZATTRACTOR_H
#define LORENZATTRACTOR_H
#include "AbstractCurve.h"

namespace Metal {
    class LorenzAttractorCurve : public AbstractCurve {
        float sigma = 10.0f;
        float rho = 28.0f;
        float beta = 8.0f / 3.0f;

        float scaleBoxMax = 10;
        glm::vec3 center = glm::vec3(scaleBoxMax/2.f);

    public:
        LorenzAttractorCurve() {
            iteration = .001;
            maxT = 1000.0f;
        }

        void registerFields() override {
            registerBaseFields();
            registerFloat(sigma, "", "Sigma", 0.1f);
            registerFloat(rho, "", "Rho", 0.1f);
            registerFloat(beta, "", "Beta", 0.01f);
            registerFloat(scaleBoxMax, "", "Caixa Máxima (N)", 1.0f);
            registerVec3(center, "", "Centro", 0, 0, true);
        }

        std::string getCurveName() override {
            return "Lorenz attractor";
        }

        glm::vec3 evaluate(float t) const override {
            return glm::vec3(1.0f, 1.0f, 1.0f);
        }

        MaxAxis addVoxels(SparseVoxelOctreeBuilder &builder) override {
            std::vector<glm::vec3> points;
            glm::vec3 point(1.0f, 1.0f, 1.0f); // Initial state
            float dt = getIteration();

            glm::vec3 minPt(FLT_MAX), maxPt(-FLT_MAX);

            for (float t = 0; t <= getMaxT(); t += dt) {
                float dx = sigma * (point.y - point.x);
                float dy = point.x * (rho - point.z) - point.y;
                float dz = point.x * point.y - beta * point.z;

                point.x += dx * dt;
                point.y += dy * dt;
                point.z += dz * dt;

                minPt = glm::min(minPt, point);
                maxPt = glm::max(maxPt, point);
                points.push_back(point);
            }

            glm::vec3 size = maxPt - minPt;
            float maxExtent = std::max({size.x, size.y, size.z});
            float scale = (maxExtent > 0.0f) ? (scaleBoxMax / maxExtent) : 1.0f;

            MaxAxis maxAxis{};

            for (auto &p : points) {
                glm::vec3 normalized = (p - minPt) * scale;

                if (normalized.x > maxAxis.x) {
                    maxAxis.x = normalized.x;
                }
                if (normalized.y > maxAxis.y) {
                    maxAxis.y = normalized.y;
                }
                if (normalized.z > maxAxis.z) {
                    maxAxis.z = normalized.z;
                }
                builder.insert(normalized);
            }
             center = glm::vec3(scaleBoxMax/2.f);

            return maxAxis;
        }
    };
}
#endif //LORENZATTRACTOR_H
