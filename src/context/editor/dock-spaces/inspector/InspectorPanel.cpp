#include "InspectorPanel.h"

#include "FilePickerUtil.h"
#include "../../abstract/form/FormPanel.h"
#include "../../../../util/UIUtil.h"
#include "../../../../common/inspection/Inspectable.h"
#include "../../../../context/ApplicationContext.h"
#include "components/TimeStampPickerPanel.h"

namespace Metal {
    void InspectorPanel::onInitialize() {
        formPanel = new FormPanel();
        timeStampPickerPanel = new TimeStampPickerPanel();
        appendChild(formPanel);
        appendChild(timeStampPickerPanel);
    }

    void InspectorPanel::renderFileSelection() const {
        if (ImGui::Button(("Selecionar arquivo" + id + "selectAudio").c_str())) {
            context->editorRepository.pathToAudio = FilePickerUtil::selectAudioFile();
            context->audioProcessorService.extractAudioData();
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

    void InspectorPanel::renderAudioInfo() {
        ImGui::Text("Informações do arquivo");
        ImGui::Separator();

        UIUtil::Spacing(true);

        ImGui::Text("Nome: %s", context->editorRepository.pathToAudio.substr(
                            context->editorRepository.pathToAudio.find_last_of('/'),
                            context->editorRepository.pathToAudio.size()).c_str());
        ImGui::Text("Tamanho: %.2f", context->editorRepository.selectedAudioSize);
        ImGui::Text("Sample rate: %d", context->editorRepository.sampleRate);
        ImGui::Text("Channels: %d", context->editorRepository.channels);
        ImGui::Text("Frames: %d", context->editorRepository.frames);
    }

    void InspectorPanel::onSync() {
        beginBox("formBase", 200);

        formPanel->setInspection(&context->editorRepository);
        formPanel->onSync();
        endBox();

        beginBox("audioSelection",40);
        renderFileSelection();
        endBox();

        if (!context->editorRepository.pathToAudio.empty()) {
            beginBox("timestampPicker", 95);
            timeStampPickerPanel->onSync();
            endBox();

            beginBox("audioInfo", 145);
            renderAudioInfo();
            endBox();
        }

        UIUtil::Spacing(true);

        if (
            // !context->editorRepository.pathToAudio.empty() &&
            ImGui::Button(("Construir representação" + id + "start").c_str())) {
            context->audioProcessorService.buildRepresentationBuffer();
        }
    }
}
