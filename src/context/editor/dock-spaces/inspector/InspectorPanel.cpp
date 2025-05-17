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
            context->editorRepository.needsDataRefresh = true;
        }
    }

    void InspectorPanel::onSync() {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), Icons::info.c_str());
        ImGui::SameLine();
        ImGui::TextWrapped("Tamanho da janela:");
        ImGui::TextWrapped("Determina a compensação entre resolução temporal e resolução de frequência.");
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), Icons::info.c_str());
        ImGui::SameLine();
        ImGui::TextWrapped("Tamanho do salto:");
        ImGui::TextWrapped("Controla a distância que você move a janela entre FFTs sucessivas.");
        ImGui::Separator();
        UIUtil::Spacing(true);

        formPanel->setInspection(&context->editorRepository);
        formPanel->onSync();

        ImGui::Separator();

        renderFileSelection();

        if (!context->editorRepository.pathToAudio.empty()) {
            ImGui::Separator();
            timeStampPickerPanel->onSync();
            ImGui::Separator();
            ImGui::Text("Frequência de Nyquist: %d", context->editorRepository.sampleRate/2);
        }
    }
}
