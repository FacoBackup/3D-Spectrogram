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
        timeStampPickerPanel = new TimeStampPickerPanel();
        appendChild(formPanel);
        appendChild(timeStampPickerPanel);
    }

    void InspectorPanel::updateAudioRelatedProperties() const {
        context->editorRepository.registerChange();
        context->editorRepository.needsDataRefresh = true;
        context->editorRepository.rangeStart = 0.f;
        context->editorRepository.rangeEnd = SAMPLE_SIZE_SECONDS / 2.f;
    }

    void InspectorPanel::renderFileSelection() const {
        if (ImGui::Button(("Selecionar arquivo" + id + "selectAudio").c_str())) {
            context->editorRepository.pathToAudio = FilePickerUtil::selectAudioFile();
            context->audioProcessorService.extractAudioData();
            updateAudioRelatedProperties();
        }
    }

    void InspectorPanel::renderStaticCurveSettings() {
        ImGui::Separator();
        ImGui::Text(("Resolução da árvore: " + std::to_string(context->editorRepository.representationResolution)).c_str());
        ImGui::Text(("Variação espacial máxima: " + std::to_string(context->editorRepository.maxDerivative)).c_str());
        ImGui::Separator();
        for (int i = 0; i < context->editorRepository.curves.size(); i++) {
            std::unique_ptr<AbstractCurve> &curve = context->editorRepository.curves.at(i);
            curve->isSelected = i == context->editorRepository.selectedCurve;
            if (ImGui::Checkbox(curve->getCurveName().c_str(), &curve->isSelected)) {
                context->editorRepository.selectedCurve = curve->isSelected ? i : -1;
                context->editorRepository.registerChange();
            }
        }
        if (context->editorRepository.selectedCurve > -1) {
            ImGui::Separator();
            std::unique_ptr<AbstractCurve> &curve = context->editorRepository.curves.at(
                context->editorRepository.selectedCurve);
            formPanel->setInspection(curve.get());
            formPanel->onSync();
            if (curve->isNotFrozen()) {
                context->editorRepository.registerChange();
                curve->freezeVersion();
            }
        }
    }

    void InspectorPanel::onSync() {
        if (ImGui::Checkbox("Mostrar curva de exemplo?", &context->editorRepository.isShowStaticCurve)) {
            context->editorRepository.pathToAudio = "";
            updateAudioRelatedProperties();
            context->editorRepository.onUpdate(nullptr, *context);
        }

        if (!context->editorRepository.isShowStaticCurve) {
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
                ImGui::Text("Frequência de Nyquist: %d", context->editorRepository.sampleRate / 2);
            }
        } else {
            renderStaticCurveSettings();
        }
    }
}
