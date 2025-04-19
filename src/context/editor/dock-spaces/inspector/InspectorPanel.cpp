#include "InspectorPanel.h"

#include "FilePickerUtil.h"
#include "../../abstract/form/FormPanel.h"
#include "../../../../util/UIUtil.h"
#include "../../../../common/inspection/Inspectable.h"
#include "../../../../context/ApplicationContext.h"
#include "../../../../service/buffer/BufferInstance.h"
#include "../../../../service/voxel/impl/SparseVoxelOctreeBuilder.h"

namespace Metal {
    void InspectorPanel::onInitialize() {
        formPanel = new FormPanel();
        appendChild(formPanel);
    }

    void InspectorPanel::onSync() {
        if (ImGui::BeginChild((id + "form").c_str())) {
            formPanel->setInspection(&context->editorRepository);
            formPanel->onSync();
        }
        if (ImGui::Button(("Build svo" + id + "start").c_str())) {
            auto builder = SparseVoxelOctreeBuilder(5, 10);
            float gridSize = 100.0f;        // Size of the X and Z grid
            float gridSize2 = gridSize * 2;        // Size of the X and Z grid
            float step = 1.0f;              // Spacing between points
            float frequency = 0.1f;         // Frequency for the sine wave
            float amplitude = 5.0f;         // Amplitude for the sine wave

            for (float x = -gridSize; x <= gridSize; x += step) {
                for (float z = -gridSize; z <= gridSize; z += step) {
                    float lX = x / gridSize2;
                    float lZ = z / gridSize2;
                    float y = sinf(x * frequency) * cosf(z * frequency) * amplitude;
                    float lY = y / gridSize2;

                    glm::vec3 point = {lX, lY, lZ};
                    VoxelData data{{1, 0, 0}}; // red color
                    builder.insert(point, data);
                }
            }

            auto voxels = builder.buildBuffer();
            context->coreBuffers.svoData->update(voxels.data());
        }

        ImGui::EndChild();
    }
}
