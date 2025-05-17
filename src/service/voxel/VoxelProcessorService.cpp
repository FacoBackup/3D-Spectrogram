#include "VoxelProcessorService.h"
#include "../audio/STFTUtil.h"
#include "../../context/ApplicationContext.h"
#include "../../service/voxel/impl/SparseVoxelOctreeBuilder.h"
#include "../buffer/BufferInstance.h"

#define WI context.editorRepository.windowIndex

namespace Metal {
    void VoxelProcessorService::process() {
        const float fScale = 8 + context.editorRepository.representationResolution;
        auto builder = SparseVoxelOctreeBuilder(WORLD_VOXEL_SCALE, fScale);

        if (context.editorRepository.needsDataRefresh) {
            std::cout << "Updating voxel data..." << std::endl;
            context.editorRepository.needsDataRefresh = false;
            context.editorRepository.audioData = context.audioProcessorService.readAudioData();
            STFTUtil::ComputeSTFT(context.editorRepository.audioData,
                                  context.editorRepository.actualWindowSize,
                                  context.editorRepository.actualHopSize,
                                  context.editorRepository.minMagnitude);
        }

        const double sampleRate = context.editorRepository.sampleRate;
        const double nyquistFrequency = sampleRate / 2.0;

        float offset = .25;
        for (const auto &point: context.editorRepository.audioData.data) {
            if (!point.frequencies.empty()) {
                if (context.editorRepository.filterWindows && (point.timestamp > WI + offset || point.timestamp < WI - offset)) {
                    continue;
                }
                for (const auto &fq: point.frequencies) {
                    if (fq.frequency > nyquistFrequency) {
                        continue;
                    }
                    const double frequency = fq.frequency / fScale;
                    int magnitudeSteps = context.editorRepository.interpolation;
                    for (int step = 1; step <= magnitudeSteps; ++step) {
                        double interpolatedMagnitude =
                                (fq.magnitude / fScale) * (static_cast<double>(step) / magnitudeSteps);
                        builder.insert({
                                           point.timestamp,
                                           interpolatedMagnitude,
                                           frequency
                                       }, VoxelData{{1, 1, 1}});
                    }
                }
            }
        }
        context.editorRepository.maxFrequency = context.editorRepository.audioData.maxFrequency / fScale;
        context.editorRepository.maxMagnitude = std::min(
            static_cast<unsigned int>(std::ceil(context.editorRepository.audioData.maxMagnitude / fScale)),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 4.f));

        context.cameraService.updateCameraTarget();

        auto voxels = builder.buildBuffer();
        context.coreBuffers.svoData->update(voxels.data());
        builder.dispose();
    }
} // Metal
