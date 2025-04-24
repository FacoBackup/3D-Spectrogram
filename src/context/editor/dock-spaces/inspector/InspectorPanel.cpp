#include "InspectorPanel.h"

#include "AudioProcessor.h"
#include "FilePickerUtil.h"
#include "../../abstract/form/FormPanel.h"
#include "../../../../util/UIUtil.h"
#include "../../../../common/inspection/Inspectable.h"
#include "../../../../context/ApplicationContext.h"
#include "../../../../service/buffer/BufferInstance.h"
#include "../../../../service/voxel/impl/SparseVoxelOctreeBuilder.h"
#include "components/TimeStampPickerPanel.h"

namespace Metal {
    void InspectorPanel::onInitialize() {
        formPanel = new FormPanel();
        timeStampPickerPanel = new TimeStampPickerPanel();
        appendChild(formPanel);
        appendChild(timeStampPickerPanel);
    }

    void InspectorPanel::renderFileSelection() {
        if (ImGui::Button(("Selecionar arquivo" + id + "selectAudio").c_str())) {
            context->editorRepository.pathToAudio = FilePickerUtil::selectAudioFile().c_str();
            if (!context->editorRepository.pathToAudio.empty()) {
                context->editorRepository.selectedAudioSize = AudioProcessor::getDuration(
                    context->editorRepository.pathToAudio);
            }
        }
        if (!context->editorRepository.pathToAudio.empty()) {
            ImGui::Text("Nome: %s", context->editorRepository.pathToAudio.substr(
                            context->editorRepository.pathToAudio.find_last_of('/'),
                            context->editorRepository.pathToAudio.size()).c_str());
            ImGui::Text("Tamanho: %s segundos", std::to_string(static_cast<int>(context->editorRepository.selectedAudioSize)).c_str());
        } else {
            ImGui::Text("Nenhum arquivo selecionado");
        }
    }

    void InspectorPanel::beginBox(std::string name, float height) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, context->themeService.palette2);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
        ImGui::BeginChild((id + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, height),
                          ImGuiChildFlags_Border);
    }

    void InspectorPanel::endBox() {
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    void InspectorPanel::onSync() {
        formPanel->setInspection(&context->editorRepository);
        formPanel->onSync();

        if (ImGui::Button(("Build svo" + id + "start").c_str())) {
            auto builder = SparseVoxelOctreeBuilder(5, 10);
            float gridSize = 100.0f; // Size of the X and Z grid
            float gridSize2 = gridSize * 2; // Size of the X and Z grid
            float step = 1.0f; // Spacing between points
            float frequency = 0.1f; // Frequency for the sine wave
            float amplitude = 5.0f; // Amplitude for the sine wave

            for (float x = 0; x <= gridSize; x += step) {
                for (float z = 0; z <= gridSize; z += step) {
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

        ImGui::NewLine();
        UIUtil::Spacing(true);
        ImGui::NewLine();

        beginBox("audioSelection", 80);
        renderFileSelection();
        endBox();

        if (!context->editorRepository.pathToAudio.empty()) {
            beginBox("timestampPicker", 85);
            timeStampPickerPanel->onSync();
            endBox();
        }

        ImGui::NewLine();
        UIUtil::Spacing(true);
    }
}
