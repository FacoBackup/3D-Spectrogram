#include "NotificationPanel.h"

#include "../ApplicationContext.h"

#define NOTIFICATION_WIDTH  250.0f
#define NOTIFICATION_HEIGHT  35.0f
#define PADDING  10.0f

namespace Metal {
    void NotificationPanel::onSync() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        long long currentTimeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime.time_since_epoch()).count();

        ImVec2 viewportPos = ImGui::GetMainViewport()->WorkPos;
        ImVec2 viewportSize = ImGui::GetMainViewport()->WorkSize;
        ImVec2 basePos = ImVec2(viewportPos.x + PADDING, viewportPos.y + viewportSize.y - PADDING);

        int index = 0;
        for (Notification &notification: context->notificationRepository.getNotifications()) {
            if (notification.stale) {
                continue;
            }
            long long elapsed = currentTimeMillis - notification.getStartTime();
            if (elapsed > 5000) {
                notification.stale = true;
                continue;
            };

            ImVec4 backgroundColor;
            switch (notification.severity) {
                case Severity::ERROR: backgroundColor = ImVec4(0.9f, 0.1f, 0.1f, 0.9f);
                    break;
                case Severity::WARNING: backgroundColor = ImVec4(0.9f, 0.7f, 0.1f, 0.9f);
                    break;
                case Severity::SUCCESS: backgroundColor = ImVec4(0.1f, 0.9f, 0.2f, 0.9f);
                    break;
                default: backgroundColor = ImVec4(0.5f, 0.5f, 0.5f, 0.9f);
                    break;
            }

            ImVec2 pos = basePos;
            pos.y -= (NOTIFICATION_HEIGHT + PADDING) * (index + 1); // Shift upwards per notification

            ImGui::SetNextWindowPos(pos);
            ImGui::SetNextWindowSize(ImVec2(NOTIFICATION_WIDTH, NOTIFICATION_HEIGHT));

            ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White text
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;
            ImGui::Begin(("##Notification" + std::to_string(index)).c_str(), nullptr, flags);
            ImGui::TextWrapped("%s", notification.message.c_str());
            ImGui::End();

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(2); // Pop both WindowBg and Text

            index++;
        }
    }
} // Metal
