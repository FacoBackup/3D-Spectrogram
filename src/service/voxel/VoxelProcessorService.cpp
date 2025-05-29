#include "VoxelProcessorService.h"
#include "../audio/STFTUtil.h"
#include "../../context/ApplicationContext.h"
#include "../../service/voxel/impl/SparseVoxelOctreeBuilder.h"
#include "../buffer/BufferInstance.h"

#define WI context.editorRepository.windowIndex

namespace Metal {
    void VoxelProcessorService::processOriginalWave(SparseVoxelOctreeBuilder &builder) {
        double amplitudeThreshold = 0.005; // Ignore amplitudes below this normalized value

        for (const auto &point: context.editorRepository.audioData.data) {
            if (point.amplitude < -amplitudeThreshold || point.amplitude > amplitudeThreshold)
                continue; // Skip insignificant amplitudes

            int magnitudeSteps = context.editorRepository.interpolation;
            for (int step = 1; step <= magnitudeSteps; ++step) {
                double interpolatedMagnitude =
                        (point.amplitude * 500.0) * (static_cast<double>(step) / magnitudeSteps);

                builder.insert({
                    (point.timestamp - context.editorRepository.rangeStart) * ORIGINAL_WAVE_SCALE,
                    interpolatedMagnitude + 5.0, // vertical offset for visibility
                    0.0
                });
            }
        }

        context.editorRepository.maxFrequency = 1;
        context.editorRepository.maxMagnitude = 10;
    }

    void VoxelProcessorService::processTransformedSignal(const float fScale, SparseVoxelOctreeBuilder &builder) {
        const double sampleRate = context.editorRepository.sampleRate;
        const double nyquistFrequency = sampleRate / 2.0;

        float offset = .25;
        for (const auto &point: context.editorRepository.audioData.data) {
            if (!point.frequencies.empty()) {
                if (context.editorRepository.filterWindows && (
                        point.timestamp > WI + offset || point.timestamp < WI - offset)) {
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
                        });
                    }
                }
            }
        }
        context.editorRepository.maxFrequency = context.editorRepository.audioData.maxFrequency / fScale;
        context.editorRepository.maxMagnitude = std::min(
            static_cast<unsigned int>(std::ceil(context.editorRepository.audioData.maxMagnitude / fScale)),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 4.f));
    }

    void VoxelProcessorService::processAudioInfo(const float fScale, SparseVoxelOctreeBuilder &builder) {
        if (context.editorRepository.isShowingOriginalWave) {
            processOriginalWave(builder);
        } else {
            processTransformedSignal(fScale, builder);
        }
    }

    void VoxelProcessorService::process() {
        const auto fScale = static_cast<float>(8 + context.editorRepository.representationResolution);
        auto builder = SparseVoxelOctreeBuilder(WORLD_VOXEL_SCALE, static_cast<int>(fScale));

        if (context.editorRepository.needsDataRefresh) {
            std::cout << "Updating voxel data..." << std::endl;
            context.editorRepository.needsDataRefresh = false;
            context.editorRepository.audioData = context.audioProcessorService.readAudioData();
            STFTUtil::ComputeSTFT(context.editorRepository.audioData,
                                  context.editorRepository.actualWindowSize,
                                  context.editorRepository.actualHopSize,
                                  context.editorRepository.minMagnitude);
        }

        if (context.editorRepository.isShowStaticCurve) {
            if (context.editorRepository.selectedCurve > -1) {
                context.editorRepository.curves[context.editorRepository.selectedCurve]->addVoxels(builder, fScale);
            }
        } else {
            processAudioInfo(fScale, builder);
        }

        context.cameraService.updateCameraTarget();

        auto voxels = builder.buildBuffer();
        context.coreBuffers.svoData->update(voxels.data());
        builder.dispose();
    }
} // Metal
