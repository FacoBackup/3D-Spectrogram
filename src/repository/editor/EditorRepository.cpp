#include "EditorRepository.h"
#include "../../common/interface/Icons.h"
#include "../../context/ApplicationContext.h"
#define SAMPLE_SIZE "Tamanho da amostra (Segundos)"
#define STFT_PARAMS "Parâmetros da transformada"

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

        actualWindowSize = static_cast<int>(1u << (windowSizeScale + 9));
        actualHopSize = actualWindowSize / hopSizeScale;
    }

    void EditorRepository::registerFields() {
        registerInt(sampleSize, "", SAMPLE_SIZE);
        registerInt(windowSizeScale, STFT_PARAMS, "Escala da janela", 1, 5);
        registerInt(actualWindowSize, STFT_PARAMS, "Tamanho da janela", 1, 5, true);
        registerInt(hopSizeScale, STFT_PARAMS, "Escala do salto", 1, 10);
        registerInt(actualHopSize, STFT_PARAMS, "Tamanho do salto", 1, 10, true);
        registerFloat(minMagnitude, STFT_PARAMS, "Magnitude minima", 0);

        registerInt(voxelSearchCount, "Debug", "Debug count divisor");
        registerBool(showRaySearchCountVoxels, "Debug", "Show ray search count");
        registerBool(showRayTestCountVoxels, "Debug", "Show ray test count");
    }
}
