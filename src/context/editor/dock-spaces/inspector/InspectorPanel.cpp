#include "InspectorPanel.h"

#include "../../../../util/FilePickerUtil.h"
#include "../../abstract/form/FormPanel.h"
#include "../../../../util/UIUtil.h"
#include "../../../../common/inspection/Inspectable.h"
#include "../../../../context/ApplicationContext.h"
#include "components/TimeStampPickerPanel.h"

namespace Metal {
    void InspectorPanel::onInitialize() {
        formPanel = new FormPanel();
        formPanel2 = new FormPanel();
        timeStampPickerPanel = new TimeStampPickerPanel();
        appendChild(formPanel);
        appendChild(formPanel2);
        appendChild(timeStampPickerPanel);
    }

    void InspectorPanel::updateAudioRelatedProperties() const {
        context->globalRepository.registerChange();
        context->globalRepository.needsDataRefresh = true;
        context->spectrogramRepository.rangeStart = 0.f;
        context->spectrogramRepository.rangeEnd = SAMPLE_SIZE_SECONDS / 2.f;
    }

    void InspectorPanel::renderFileSelection() const {
        if (ImGui::Button(("Selecionar arquivo" + id + "selectAudio").c_str())) {
            context->spectrogramRepository.pathToAudio = FilePickerUtil::selectAudioFile();
            context->audioProcessorService.extractAudioData();
            updateAudioRelatedProperties();
        }
    }

    void InspectorPanel::renderStaticCurveSettings() {
        for (int i = 0; i < context->globalRepository.curves.size(); i++) {
            std::unique_ptr<AbstractCurve> &curve = context->globalRepository.curves.at(i);
            curve->isSelected = i == context->globalRepository.selectedCurve;
            if (ImGui::Checkbox(curve->getCurveName().c_str(), &curve->isSelected)) {
                context->globalRepository.selectedCurve = curve->isSelected ? i : -1;
                context->globalRepository.registerChange();
            }
        }
        if (context->globalRepository.selectedCurve > -1) {
            ImGui::Separator();
            std::unique_ptr<AbstractCurve> &curve = context->globalRepository.curves.at(
                context->globalRepository.selectedCurve);
            formPanel2->setInspection(curve.get());
            formPanel2->onSync();
            if (curve->isNotFrozen()) {
                context->globalRepository.registerChange();
                curve->freezeVersion();
            }
        }
    }

    void InspectorPanel::onSync() {

        formPanel->setInspection(&context->globalRepository);
        formPanel->onSync();

        ImGui::Separator();

        if (!context->globalRepository.isShowStaticCurve) {


            formPanel2->setInspection(&context->spectrogramRepository);
            formPanel2->onSync();
            if(ImGui::CollapsingHeader("Ajuda")) {
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
            }
            ImGui::Separator();

            renderFileSelection();

            if (!context->spectrogramRepository.pathToAudio.empty()) {
                ImGui::Separator();
                timeStampPickerPanel->onSync();
                ImGui::Separator();
                ImGui::Text("Frequência de Nyquist: %d", context->spectrogramRepository.sampleRate / 2);
            }
        } else {
            renderStaticCurveSettings();
        }
    }
}
