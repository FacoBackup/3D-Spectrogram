#ifndef AUDIODATA_H
#define AUDIODATA_H

#include <vector>

namespace Metal {
    struct AudioDataPoint {
        double timestamp;
        double amplitude;
        std::vector<double> frequencies;
    };

    using AudioDataVector = std::vector<AudioDataPoint>;
}

#endif
