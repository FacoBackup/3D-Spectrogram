#include "EditorRepository.h"
#include "../../common/interface/Icons.h"
#include "../../context/ApplicationContext.h"
#define SAMPLE_SIZE "Tamanho da amostra (Segundos)"

namespace Metal {
    const char *EditorRepository::getTitle() {
        return "Editor";
    }

    const char *EditorRepository::getIcon() {
        return Icons::settings.c_str();
    }

    void EditorRepository::onUpdate(InspectableMember *member, ApplicationContext &context) {
        if (member->name == SAMPLE_SIZE) {
            if (!pathToAudio.empty() && static_cast<int>(selectedAudioSize) < sampleSize) {
                pathToAudio = "";
                context.notificationRepository.addNotification("Audio selecionado é menor que o tamanho da amostra");
            }
            rangeEnd = static_cast<float>(sampleSize);
            rangeStart = 0;
            worldSize = sampleSize * 2;
            context.engineContext.camera.target = glm::vec3(static_cast<float>(sampleSize));
            context.engineContext.camera.changed = true;
            context.engineContext.setCameraUpdated(true);
        }
    }

    void EditorRepository::registerFields() {
        registerInt(sampleSize, "", SAMPLE_SIZE);
        registerInt(voxelSearchCount, "", "Debug count divisor");
        registerBool(showRaySearchCountVoxels, "", "Show ray search count");
        registerBool(showRayTestCountVoxels, "", "Show ray test count");
    }
}
