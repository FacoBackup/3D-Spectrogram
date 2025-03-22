#ifndef TCC_IESB_GUICONTEXT_H
#define TCC_IESB_GUICONTEXT_H

#define LARGE_FONT_SIZE 38
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include "../../common/AbstractRuntimeComponent.h"

namespace Metal {
    struct FrameBufferAttachment;

    class GuiContext final : public AbstractRuntimeComponent {
        static void applySpacing();

        void applyFonts();

        ImDrawData *drawData = nullptr;
        ImFont *largeIconsFont = nullptr;

    public:
        explicit GuiContext(ApplicationContext &context);

        [[ nodiscard]] ImFont *getLargeIconsFont() const {
            return largeIconsFont;
        }

        void dispose() const;

        void onInitialize() override;

        void endFrame();

        static void BeginFrame();

        static void RecordImguiCommandBuffer(ImDrawData *drawData, VkResult &err, ImGui_ImplVulkanH_Window &wd,
                                             ImGui_ImplVulkanH_Frame *fd);
    };
}

#endif
