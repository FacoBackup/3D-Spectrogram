#include "InspectorPanel.h"

#include "MaterialInspection.h"
#include "../../abstract/form/FormPanel.h"
#include "../../../../util/UIUtil.h"
#include "../../../../common/inspection/Inspectable.h"
#include "../../../../context/ApplicationContext.h"
#include "../../../../repository/world/impl/Entity.h"
#include "../../../../repository/world/impl/AbstractComponent.h"
#include "../../../../service/camera/Camera.h"

namespace Metal {
    void InspectorPanel::onInitialize() {
        formPanel = new FormPanel();
        appendChild(formPanel);
        repositories.push_back(&context->editorRepository);
        repositories.push_back(&context->engineRepository);
        repositories.push_back(&context->worldRepository.camera);
        repositories.push_back(&context->fileInspection);
    }

    void InspectorPanel::onSync() {
        const auto &editorRepository = context->editorRepository;
        ImGui::Columns(2, (id + "inspectorColumns").c_str(), false);
        ImGui::SetColumnWidth(0, 30);
        for (auto *repo: repositories) {
            if (UIUtil::RenderOption(repo->getIcon(), currentInspection == repo, true,
                                     editorRepository.accent)) {
                currentInspection = repo;
            }
            UIUtil::RenderTooltip(repo->getTitle());
        }

        ImGui::Spacing();
        ImGui::Spacing();
        for (auto *additional: additionalInspection) {
            if (additional != nullptr) {
                if (UIUtil::RenderOption(
                    std::format("{}##{}", additional->getIcon(), additional->getUniqueId()),
                    currentInspection == additional,
                    true,
                    editorRepository.accent)) {
                    currentInspection = additional;
                }
                UIUtil::RenderTooltip(additional->getTitle());
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::NextColumn();
        if (ImGui::BeginChild((id + "form").c_str())) {
            formPanel->setInspection(currentInspection);
            formPanel->onSync();
        }
        ImGui::EndChild();
        ImGui::Columns(1);
    }
}
