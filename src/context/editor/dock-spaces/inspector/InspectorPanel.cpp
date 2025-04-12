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
            float divisor = 5000;
            for (float i = 0; i < 5000; i++) {
                VoxelData data{{1, 0, 0}};
                glm::vec3 point = {i / divisor, sin(i), 0};
                builder.insert(point, data);
            }

            auto voxels = builder.buildBuffer();
            context->coreBuffers.svoData->update(voxels.data());
        }

        ImGui::EndChild();
    }
}
