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

        context.editorRepository.maxZAxis = 1;
        context.editorRepository.maxYAxis = 10;
    }

    int VoxelProcessorService::estimateSpectrogramOctreeDepth() {
        float maxDx = 0.0f, maxDy = 0.0f, maxDz = 0.0f;

        glm::vec3 lastPoint{0.0f};
        bool first = true;

        for (const auto &point : context.editorRepository.audioData.data) {
            if (point.frequencies.empty()) continue;

            for (const auto &fq: point.frequencies) {
                if (fq.frequency > (context.editorRepository.sampleRate / (2.0 * WORLD_VOXEL_SCALE))) continue;

                const double frequency = fq.frequency;
                double interpolatedMagnitude = fq.magnitude;

                glm::vec3 currentPoint{
                    point.timestamp,
                    interpolatedMagnitude,
                    frequency
                };

                if (!first) {
                    glm::vec3 delta = glm::abs(currentPoint - lastPoint);
                    maxDx = std::max(maxDx, delta.x);
                    maxDy = std::max(maxDy, delta.y);
                    maxDz = std::max(maxDz, delta.z);
                }
                lastPoint = currentPoint;
                first = false;
            }
        }

        float maxRate = std::max({maxDx, maxDy, maxDz});
        if (maxRate == 0.0f) return 1; // Flat data fallback

        float voxelSize = 1.0f / (2.0f * maxRate * WORLD_VOXEL_SCALE); // Nyquist voxel size
        int depth = static_cast<int>(std::ceil(std::log2(1. / voxelSize)));
        return std::clamp(depth, 1, 12);
    }


    void VoxelProcessorService::processSpectrogram(const float fScale, SparseVoxelOctreeBuilder &builder) {
        const double sampleRate = context.editorRepository.sampleRate;
        const double nyquistFrequency = sampleRate / (WORLD_VOXEL_SCALE * 2.0);

        STFTUtil::ComputeSTFT(context.editorRepository.audioData,
                              context.editorRepository.actualWindowSize,
                              context.editorRepository.actualHopSize,
                              context.editorRepository.minMagnitude);

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
        context.editorRepository.maxZAxis = nyquistFrequency / fScale;
        context.editorRepository.maxYAxis = static_cast<unsigned int>(std::ceil(
            context.editorRepository.audioData.maxMagnitude / fScale));
    }

    void VoxelProcessorService::processAudioInfo(const float fScale, SparseVoxelOctreeBuilder &builder) {
        if (context.editorRepository.isShowingOriginalWave) {
            processOriginalWave(builder);
        } else {
            processSpectrogram(fScale, builder);
        }
    }

    int VoxelProcessorService::estimateMinimumDepth(AbstractCurve &curve) const {
        if (!curve.hasDerivative()) {
            return 12;
        }

        float dt = curve.getIteration();
        float maxT = curve.getMaxT();
        float &maxDerivative = context.editorRepository.maxDerivative;
        maxDerivative = 0.0f;

        glm::vec3 prev = curve.evaluate(0.0f);
        for (float t = dt; t <= maxT; t += dt) {
            glm::vec3 curr = curve.evaluate(t);
            float derivative = glm::length((curr - prev) / dt);
            maxDerivative = std::max(maxDerivative, derivative);
            prev = curr;
        }

        float vmin = 1.0f / (2.0f * maxDerivative * WORLD_VOXEL_SCALE); // Nyquist voxel size
        int depth = static_cast<int>(std::ceil(std::log2(1. / vmin)));
        return std::clamp(depth, 1, 12); // Clamp to [1, 12] for safety
    }

    void VoxelProcessorService::refreshData() const {
        if (context.editorRepository.needsDataRefresh && !context.editorRepository.pathToAudio.empty()) {
            std::cout << "Updating voxel data..." << std::endl;
            context.editorRepository.needsDataRefresh = false;
            context.editorRepository.audioData = context.audioProcessorService.readAudioData();
            STFTUtil::ComputeSTFT(context.editorRepository.audioData,
                                  context.editorRepository.actualWindowSize,
                                  context.editorRepository.actualHopSize,
                                  context.editorRepository.minMagnitude);
        }
    }

    void VoxelProcessorService::process() {
        refreshData();
        SparseVoxelOctreeBuilder *builder = nullptr;

        if (context.editorRepository.isShowStaticCurve) {
            if (context.editorRepository.selectedCurve > -1) {
                std::unique_ptr<AbstractCurve> &curve = context
                        .editorRepository
                        .curves[context.editorRepository.selectedCurve];
                context.editorRepository.actualTreeDepth = estimateMinimumDepth(*curve.get());
                builder = new SparseVoxelOctreeBuilder(
                    WORLD_VOXEL_SCALE, context.editorRepository.actualTreeDepth);

                MaxAxis maxAxis = curve->addVoxels(*builder);
                context.editorRepository.maxXAxis = maxAxis.x + 1;
                context.editorRepository.maxYAxis = maxAxis.y + 1;
                context.editorRepository.maxZAxis = maxAxis.z + 1;
            }
        } else {
            context.editorRepository.actualTreeDepth = context.editorRepository.representationResolution;
            if (context.editorRepository.useNyquistForSpectrogramDepth) {
                context.editorRepository.actualTreeDepth = estimateSpectrogramOctreeDepth();
            }
            float fScale = static_cast<float>(context.editorRepository.actualTreeDepth);

            builder = new SparseVoxelOctreeBuilder(WORLD_VOXEL_SCALE, static_cast<int>(fScale));
            processAudioInfo(fScale, *builder);
        }

        context.editorRepository.maxXAxis = std::min(
            static_cast<unsigned int>(context.editorRepository.maxXAxis),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 2));
        context.editorRepository.maxYAxis = std::min(
            static_cast<unsigned int>(context.editorRepository.maxYAxis),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 2));
        context.editorRepository.maxZAxis = std::min(
            static_cast<unsigned int>(context.editorRepository.maxZAxis),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 2));

        context.cameraService.updateCameraTarget();

        if (builder != nullptr) {
            auto voxels = builder->buildBuffer();
            context.coreBuffers.svoData->update(voxels.data());
            builder->dispose();
            delete builder;
        }
    }
} // Metal
