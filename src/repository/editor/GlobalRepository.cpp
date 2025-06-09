#include "GlobalRepository.h"
#include "../../common/interface/Icons.h"
#define DEBUG "Debug"

namespace Metal {
    const char *GlobalRepository::getTitle() {
        return "Configurações Globais";
    }

    const char *GlobalRepository::getIcon() {
        return Icons::settings.c_str();
    }

    void GlobalRepository::onUpdate(InspectableMember *member, ApplicationContext &context) {
        needsDataRefresh = true;
    }

    void GlobalRepository::registerFields() {
        registerInt(actualTreeDepth, "", "Resolução da árvore", 6, 13);
        registerFloat(maxDerivative, "", "Variação espacial máxima", 0, 0, true);
        registerInt(voxelCount, "", "Quantidade de voxels", 0, 0, true);
        registerText(memorySize, "", "Uso de memória", true);
        registerBool(isShowStaticCurve, "", "Mostrar curva de exemplo?");
        registerBool(useNyquistForTreeDepth, "", "Utilizar nyquist para resolução da árvore?");


        registerInt(voxelSearchCount, DEBUG, "Debug count divisor");
        registerBool(showRaySearchCountVoxels, DEBUG, "Show ray search count");
        registerBool(showRayTestCountVoxels, DEBUG, "Show ray test count");
    }
} // Metal
