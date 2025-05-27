#include "EditorRepository.h"
#include "../../common/interface/Icons.h"
#include "../../context/ApplicationContext.h"
#define STFT_PARAMS "Parâmetros da transformada"
#define RENDERING_PARAMS "Renderização"
#define FILTERING_PARAMS "Filtro"
#define FILE_INFO "Informações do arquivo"
#define DEBUG "Debug"

namespace Metal {
    const char *EditorRepository::getTitle() {
        return "Editor";
    }

    const char *EditorRepository::getIcon() {
        return Icons::settings.c_str();
    }

    void EditorRepository::onUpdate(InspectableMember *member, ApplicationContext &context) {
        actualWindowSize = ACTUAL_WINDOW_SIZE;
        interpolation = ACTUAL_INTERPOLATION;
        actualHopSize = actualWindowSize / hopSizeScale;

        if (member->group != RENDERING_PARAMS && member->group != FILTERING_PARAMS) {
            needsDataRefresh = true;
        }
        if (isShowingOriginalWave) {
            interpolationScale = 1;
            representationResolution = 6;
        } else {
            interpolationScale = 8;
            representationResolution = 2;
        }
    }

    void EditorRepository::registerFields() {
        registerBool(isShowingOriginalWave, "", "Mostrar onda original?");
        registerFloat(minMagnitude, STFT_PARAMS, "Magnitude minima", 0);
        registerInt(windowSizeScale, STFT_PARAMS, "Escala da janela", 1, 5);
        registerInt(hopSizeScale, STFT_PARAMS, "Escala do salto", 1, 10);
        registerInt(actualWindowSize, STFT_PARAMS, "Tamanho da janela", 1, 5, true);
        registerInt(actualHopSize, STFT_PARAMS, "Tamanho do salto", 1, 10, true);

        registerBool(filterWindows, FILTERING_PARAMS, "Filtrar janela da análise?");
        registerInt(windowIndex, FILTERING_PARAMS, "Janela da análise", 0, SAMPLE_SIZE_SECONDS);

        registerInt(interpolationScale, RENDERING_PARAMS, "Escala da interpolação (Impacta desempenho)", 1);
        registerInt(representationResolution, RENDERING_PARAMS, "Resolução da representação (Impacta desempenho)", 1,
                    10);
        registerInt(interpolation, RENDERING_PARAMS, "Interpolação das amostras", 1, 100, true);

        registerInt(voxelSearchCount, DEBUG, "Debug count divisor");
        registerBool(showRaySearchCountVoxels, DEBUG, "Show ray search count");
        registerBool(showRayTestCountVoxels, DEBUG, "Show ray test count");


        registerText(pathToAudio, FILE_INFO, "Caminho do arquivo", true);
        registerFloat(selectedAudioSize, FILE_INFO, "Tamanho (segundos)", 0, 0, true);
        registerInt(channels, FILE_INFO, "Canais", 0, 0, true);
        registerInt(frames, FILE_INFO, "Frames", 0, 0, true);
        registerInt(sampleRate, FILE_INFO, "Sample rate", 0, 0, true);
    }
}
