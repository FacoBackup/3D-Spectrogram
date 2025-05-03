#include "VoxelProcessorService.h"
#include "../audio/STFTUtil.h"
#include "../../context/ApplicationContext.h"
#include "../../service/voxel/impl/SparseVoxelOctreeBuilder.h"
#include "../buffer/BufferInstance.h"

namespace Metal {
    void VoxelProcessorService::process() {
        const float fScale = 10;
        unsigned int maxWorldSize = context.editorRepository.sampleSize * WORLD_VOXEL_SCALE;
        auto builder = SparseVoxelOctreeBuilder(maxWorldSize, fScale);

        auto audioData = context.audioProcessorService.readAudioData();
        STFTUtil::ComputeSTFT(audioData,
                              context.editorRepository.actualWindowSize,
                              context.editorRepository.actualHopSize,
                              context.editorRepository.minMagnitude);

        for (const auto &point: audioData.data) {
            auto timestamp = (point.timestamp - context.editorRepository.rangeStart);
            if (!point.frequencies.empty()) {
                for (const auto &fq: point.frequencies) {
                    const double frequency = fq.frequency / fScale;
                    int magnitudeSteps = context.editorRepository.interpolation;
                    for (int step = 1; step <= magnitudeSteps; ++step) {
                        double interpolatedMagnitude =
                                (fq.magnitude / fScale) * (static_cast<double>(step) / magnitudeSteps);
                        builder.insert({
                                           timestamp,
                                           interpolatedMagnitude,
                                           frequency
                                       }, VoxelData{{1, 1, 1}});
                    }
                }
            }
        }
        context.editorRepository.maxFrequency = std::min(static_cast<unsigned int>(audioData.maxFrequency / fScale),
                                                         maxWorldSize / 2);
        context.editorRepository.maxMagnitude = std::min(
            static_cast<unsigned int>(std::ceil(audioData.maxMagnitude / fScale)),
            maxWorldSize / 2);

        context.cameraService.updateCameraTarget();

        auto voxels = builder.buildBuffer();
        context.coreBuffers.svoData->update(voxels.data());
    }
} // Metal
