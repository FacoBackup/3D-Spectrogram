#include "EditorRepository.h"
#include "../../common/interface/Icons.h"

namespace Metal {
    const char *EditorRepository::getTitle() {
        return "Editor";
    }

    const char *EditorRepository::getIcon() {
        return Icons::settings.c_str();
    }

    void EditorRepository::registerFields() {
        // registerColor(backgroundColor, "", "backgroundColor");

        registerInt(voxelSearchCount, "", "Debug count divisor");
        registerBool(showRaySearchCountVoxels, "", "Show ray search count");
        registerBool(showRayTestCountVoxels, "", "Show ray test count");
    }
}
