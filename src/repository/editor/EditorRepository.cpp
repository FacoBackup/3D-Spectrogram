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
            context.cameraService.updateCameraTarget();
        }

        actualWindowSize = static_cast<int>(1u << (context.editorRepository.windowSize + 9));
        if (context.engineContext.camera.isOrthographic != isOrthographic) {
            context.engineContext.camera.isOrthographic = isOrthographic;
            context.cameraService.updateCameraTarget();
        }
    }

    void EditorRepository::registerFields() {
        registerInt(sampleSize, "", SAMPLE_SIZE);
        registerInt(windowSize, "", "Tamanho da janela de análise", 1, 5);
        registerInt(actualWindowSize, "", "Total de amostras da janela", 1, 5, true);
        registerBool(showOriginalWave, "", "Mostrar onda original");
        registerFloat(minMagnitude, "", "Magnitude minima", 0);
        registerBool(isOrthographic, "", "Projeção ortográfica?");

        registerInt(voxelSearchCount, "Debug", "Debug count divisor");
        registerBool(showRaySearchCountVoxels, "Debug", "Show ray search count");
        registerBool(showRayTestCountVoxels, "Debug", "Show ray test count");
    }
}
