#include "SpectrogramRepository.h"
#include "../../common/interface/Icons.h"
#include "../../context/ApplicationContext.h"
#define STFT_PARAMS "Parâmetros da transformada"
#define RENDERING_PARAMS "Renderização"
#define FILTERING_PARAMS "Filtro"
#define FILE_INFO "Informações do arquivo"
#define SHOW_ORIGINAL_WAVE "Mostrar onda original?"

namespace Metal {
    const char *SpectrogramRepository::getTitle() {
        return "Configuração Espectrograma";
    }

    const char *SpectrogramRepository::getIcon() {
        return Icons::settings.c_str();
    }

    void SpectrogramRepository::onUpdate(InspectableMember *member, ApplicationContext &context) {
        actualWindowSize = ACTUAL_WINDOW_SIZE;
        interpolation = ACTUAL_INTERPOLATION;
        actualHopSize = actualWindowSize / hopSizeScale;

        if (member != nullptr && member->group != RENDERING_PARAMS && member->group != FILTERING_PARAMS) {
            context.globalRepository.needsDataRefresh = true;
        }

        if (member != nullptr && member->name == SHOW_ORIGINAL_WAVE) {
            if (isShowingOriginalWave) {
                interpolationScale = 1;
                context.globalRepository.actualTreeDepth = 12;
                return;
            }
            interpolationScale = 8;
            context.globalRepository.actualTreeDepth = 10;
        }
    }

    void SpectrogramRepository::registerFields() {
        registerBool(isShowingOriginalWave, "", SHOW_ORIGINAL_WAVE);
        registerFloat(minMagnitude, STFT_PARAMS, "Magnitude minima", 0);
        registerInt(windowSizeScale, STFT_PARAMS, "Escala da janela", 1, 5);
        registerInt(hopSizeScale, STFT_PARAMS, "Escala do salto", 1, 10);
        registerInt(actualWindowSize, STFT_PARAMS, "Tamanho da janela", 1, 5, true);
        registerInt(actualHopSize, STFT_PARAMS, "Tamanho do salto", 1, 10, true);

        registerBool(filterWindows, FILTERING_PARAMS, "Filtrar janela da análise?");
        registerInt(windowIndex, FILTERING_PARAMS, "Janela da análise", 0, SAMPLE_SIZE_SECONDS);

        registerInt(interpolationScale, RENDERING_PARAMS, "Escala da interpolação (Impacta desempenho)", 1);
        registerInt(interpolation, RENDERING_PARAMS, "Interpolação das amostras", 1, 100, true);

        registerText(pathToAudio, FILE_INFO, "Caminho do arquivo", true);
        registerInt(channels, FILE_INFO, "Canais", 0, 0, true);
        registerInt(frames, FILE_INFO, "Frames", 0, 0, true);
        registerInt(sampleRate, FILE_INFO, "Sample rate", 0, 0, true);
    }
}
