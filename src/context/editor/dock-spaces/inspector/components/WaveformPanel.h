#ifndef WAVEFORMPANEL_H
#define WAVEFORMPANEL_H
#include "../../../abstract/AbstractPanel.h"

namespace Metal {
    class WaveformPanel final : public AbstractPanel {
        std::vector<float> cachedData;
        float zoom = 10.0f;
        float scroll = 0.0f;
        float lastZoom = -1.0f;
        float lastScroll = -1.0f;

    public:
        void updateCachedData();
        void onSync() override;

    };
} // Metal

#endif //WAVEFORMPANEL_H
