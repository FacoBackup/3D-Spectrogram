#ifndef THEMESERVICE_H
#define THEMESERVICE_H
#include <imgui.h>
#include <glm/vec3.hpp>
#include "../../common/AbstractRuntimeComponent.h"


namespace Metal {
    class ThemeService final : public AbstractRuntimeComponent {
    public:
        ImVec4 neutralPalette{};
        ImVec4 palette0{};
        ImVec4 palette1{};
        ImVec4 palette2{};
        ImVec4 palette3{};
        ImVec4 palette4{};
        ImVec4 palette5{};
        ImVec4 palette6{};
        ImVec4 textDisabled{};
        ImVec4 accentColor{0.26f, 0.59f, 0.98f, 1};
        bool isDone = false;
        static glm::vec3 BACKGROUND_COLOR;
        float prevLength = -1.f;

        explicit ThemeService(ApplicationContext &context) : AbstractRuntimeComponent(context) {
        }

        void onSync() override;
    };
} // Metal

#endif
