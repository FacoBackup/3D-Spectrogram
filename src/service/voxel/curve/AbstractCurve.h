#ifndef ABSTRACTCURVE_H
#define ABSTRACTCURVE_H
#include "../../../common/inspection/Inspectable.h"
#include "../impl/SparseVoxelOctreeBuilder.h"

namespace Metal {
    class SparseVoxelOctreeBuilder;

    struct MaxAxis {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    class AbstractCurve : public Inspectable {
    protected:
        float maxT = 10.0f;
        float iteration = .001;

        virtual glm::vec3 evaluate(float t) const {
            return glm::vec3(0.0f);
        }

    public:
        bool isSelected = false;

        virtual std::string getCurveName() {
            throw std::logic_error("Not implemented");
        }

        void registerBaseFields() {
            registerFloat(iteration, "", "Tamanho da iteração", 0.001f);
            registerFloat(maxT, "", "Valor máximo de t", 0.001f);
        }

        float getMaxT() {
            return maxT;
        }

        float getIteration() {
            return iteration;
        }

        virtual MaxAxis addVoxels(SparseVoxelOctreeBuilder &builder) {
            MaxAxis maxAxis{};
            for (float t = 0.0f; t <= getMaxT(); t += getIteration()) {
                glm::vec3 point = evaluate(t);
                if (point.x > maxAxis.x) {
                    maxAxis.x = point.x;
                }
                if (point.y > maxAxis.y) {
                    maxAxis.y = point.y;
                }
                if (point.z > maxAxis.z) {
                    maxAxis.z = point.z;
                }
                builder.insert(point);
            }
            return maxAxis;
        }
    };
}

#endif //ABSTRACTCURVE_H
