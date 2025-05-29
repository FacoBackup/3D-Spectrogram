#ifndef ABSTRACTCURVE_H
#define ABSTRACTCURVE_H
#include "../../../common/inspection/Inspectable.h"
#include "../impl/SparseVoxelOctreeBuilder.h"

namespace Metal {
    class SparseVoxelOctreeBuilder;

    class AbstractCurve : public Inspectable {
        float maxX = 10;
        float maxY = 10;
        float maxZ = 10;
        float iteration = .1;

    protected:
        virtual float runFunctionX(float axisData) {
            return 0.0f;
        }

        virtual float runFunctionZ(float axisData) {
            return 0.0f;
        }

        virtual float runFunctionY(float axisData) {
            return 0.0f;
        }

    public:
        bool isSelected = false;

        virtual std::string getName() {
            throw std::logic_error("Not implemented");
        }

        void registerBaseFields() {
            registerFloat(iteration, "", "Tamanho da iteração", .0001);

            registerFloat(maxX, "", "Max X", .001);
            registerFloat(maxY, "", "Max Y", .001);
            registerFloat(maxZ, "", "Max Z", .001);
        }

        void addVoxels(SparseVoxelOctreeBuilder &builder, float fScale) {
            for (float x = 0; x < maxX; x += iteration) {
                for (float y = 0; y < maxY; y += iteration) {
                    for (float z = 0; z < maxZ; z += iteration) {
                        builder.insert({runFunctionX(x) / fScale, runFunctionY(y) / fScale, runFunctionZ(z) / fScale});
                    }
                }
            }
        }
    };
}

#endif //ABSTRACTCURVE_H
