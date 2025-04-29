#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include <sndfile.h>
#include "AudioData.h"
#include "../abstract/AbstractResourceService.h"

namespace Metal {
    class SparseVoxelOctreeBuilder;

    struct AudioProcessorService final : AbstractResourceService {
        explicit AudioProcessorService(ApplicationContext &context)
            : AbstractResourceService(context) {
        }

        AudioDataVector readAudioData() const;

        void extractAudioData() const;

        void buildRepresentationBuffer();
    };
};

#endif //AUDIOPROCESSOR_H
