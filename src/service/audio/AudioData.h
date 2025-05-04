#ifndef AUDIODATA_H
#define AUDIODATA_H

#include <vector>

namespace Metal {
    struct Frequency {
        unsigned int frequency;
        double magnitude;
    };

    struct AudioDataPoint {
        double timestamp;
        double amplitude;
        std::vector<Frequency> frequencies;
    };

    struct AudioDataVector {
        std::vector<AudioDataPoint> data;
        unsigned int maxFrequency;
        double maxMagnitude;
    };
}

#endif
