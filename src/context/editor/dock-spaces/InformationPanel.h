#ifndef INFORMATIONPANEL_H
#define INFORMATIONPANEL_H
#include <imgui.h>

#include "../abstract/AbstractPanel.h"

namespace Metal {
    struct AxisLAbel {
        ImVec4 color;
        std::string label;
    };

    class InformationPanel final : public AbstractPanel {
        std::vector<AxisLAbel> spectrogramLabels = {
            {ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tempo"},
            {ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Frequência"},
            {ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Magnitude"}
        };

        std::vector<AxisLAbel> functionLabels = {
            {ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X"},
            {ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y"},
            {ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Z"}
        };

    public:
        static void renderLabel(const AxisLAbel &item);

        void onSync() override;
    };
} // Metal

#endif //INFORMATIONPANEL_H
