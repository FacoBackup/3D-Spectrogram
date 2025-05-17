#include "WaveformPanel.h"

#include "../../../../ApplicationContext.h"

namespace Metal {
    void WaveformPanel::updateCachedData() {
        zoom = std::max(zoom, .01f);
        const auto &points = context->editorRepository.audioData.data;
        int totalPoints = static_cast<int>(points.size());
        int visiblePoints = std::max(1, static_cast<int>(totalPoints / zoom));
        int start = static_cast<int>((totalPoints - visiblePoints) * scroll);
        int end = std::min(start + visiblePoints, totalPoints);

        int screenWidth = static_cast<int>(ImGui::GetContentRegionAvail().x);
        int step = std::max(1, (end - start) / screenWidth);

        cachedData.clear();
        cachedData.reserve((end - start) / step + 1);

        for (int i = start; i < end; i += step) {
            double sum = 0;
            int count = 0;
            for (int j = 0; j < step && i + j < end; ++j) {
                sum += points[i + j].amplitude;
                ++count;
            }
            cachedData.push_back(static_cast<float>(sum / count));
        }

        lastZoom = zoom;
        lastScroll = scroll;
    }

    void WaveformPanel::onSync() {
        if (context->editorRepository.audioData.data.empty()) {
            ImGui::Text("No audio data.");
            return;
        }

        bool dirty = false;

        dirty |= ImGui::SliderFloat("Scroll", &scroll, 0.0f, 1.0f);

        if (dirty || zoom != lastZoom || scroll != lastScroll) {
            updateCachedData();
        }

        ImGui::PlotLines("Onda do sinal", cachedData.data(), cachedData.size(), 0, nullptr, -1.0f, 1.0f, ImVec2(0, 100));
    }
} // Metal
