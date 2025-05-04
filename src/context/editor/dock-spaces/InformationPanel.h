#ifndef INFORMATIONPANEL_H
#define INFORMATIONPANEL_H
#include <imgui.h>

#include "../abstract/AbstractPanel.h"

namespace Metal {
    struct ColorLabel {
        ImVec4 color;
        std::string label;
    };

    class InformationPanel final : public AbstractPanel {
        std::vector<ColorLabel> colorLabels = {
            {ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tempo"},
            {ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Frequência"},
            {ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Magnitude"}
        };

    public:
        void onSync() override;
    };
} // Metal

#endif //INFORMATIONPANEL_H
