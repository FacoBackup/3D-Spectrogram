#include "VoxelProcessorService.h"
#include "../audio/STFTUtil.h"
#include "../../context/ApplicationContext.h"
#include "../../service/voxel/impl/SparseVoxelOctreeBuilder.h"
#include "../buffer/BufferInstance.h"

#define WI context.spectrogramRepository.windowIndex

namespace Metal {
    void VoxelProcessorService::processOriginalWave(SparseVoxelOctreeBuilder &builder) {
        double amplitudeThreshold = 0.005; // Ignore amplitudes below this normalized value

        for (const auto &point: context.spectrogramRepository.audioData.data) {
            if (point.amplitude < -amplitudeThreshold || point.amplitude > amplitudeThreshold)
                continue; // Skip insignificant amplitudes

            int magnitudeSteps = context.spectrogramRepository.interpolation;
            for (int step = 1; step <= magnitudeSteps; ++step) {
                double interpolatedMagnitude =
                        (point.amplitude * 500.0) * (static_cast<double>(step) / magnitudeSteps);

                builder.insert({
                    (point.timestamp - context.spectrogramRepository.rangeStart) * ORIGINAL_WAVE_SCALE,
                    interpolatedMagnitude + 5.0, // vertical offset for visibility
                    0.0
                });
            }
        }

        context.globalRepository.maxZAxis = 1;
        context.globalRepository.maxYAxis = 10;
    }

    int VoxelProcessorService::estimateSpectrogramOctreeDepth() const {
        float maxDx = 0.0f, maxDy = 0.0f, maxDz = 0.0f;

        glm::vec3 lastPoint{0.0f};
        bool first = true;

        for (const auto &point : context.spectrogramRepository.audioData.data) {
            if (point.frequencies.empty()) continue;

            for (const auto &fq: point.frequencies) {
                if (fq.frequency > (context.spectrogramRepository.sampleRate / (2.0 * WORLD_VOXEL_SCALE))) continue;

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
        context.globalRepository.maxDerivative = maxRate;
        float voxelSize = 1.0f / (2.0f * maxRate * WORLD_VOXEL_SCALE); // Nyquist voxel size
        int depth = static_cast<int>(std::ceil(std::log2(1. / voxelSize)));
        return std::clamp(depth, 1, 12);
    }


    void VoxelProcessorService::processSpectrogram(const float fScale, SparseVoxelOctreeBuilder &builder) {
        const double sampleRate = context.spectrogramRepository.sampleRate;
        const double nyquistFrequency = sampleRate / (WORLD_VOXEL_SCALE * 2.0);

        float offset = .25;
        for (const auto &point: context.spectrogramRepository.audioData.data) {
            if (!point.frequencies.empty()) {
                if (context.spectrogramRepository.filterWindows && (
                        point.timestamp > WI + offset || point.timestamp < WI - offset)) {
                    continue;
                }
                for (const auto &fq: point.frequencies) {
                    if (fq.frequency > nyquistFrequency) {
                        continue;
                    }
                    const double frequency = fq.frequency / fScale;
                    int magnitudeSteps = context.spectrogramRepository.interpolation;
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
        context.globalRepository.maxZAxis = nyquistFrequency / fScale;
        context.globalRepository.maxYAxis = static_cast<unsigned int>(std::ceil(
            context.spectrogramRepository.audioData.maxMagnitude / fScale));
    }

    void VoxelProcessorService::processAudioInfo(const float fScale, SparseVoxelOctreeBuilder &builder) {
        if (context.spectrogramRepository.isShowingOriginalWave) {
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
        float &maxDerivative = context.globalRepository.maxDerivative;
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
        if (context.globalRepository.needsDataRefresh && !context.spectrogramRepository.pathToAudio.empty()) {
            std::cout << "Updating voxel data..." << std::endl;
            context.globalRepository.needsDataRefresh = false;
            context.spectrogramRepository.audioData = context.audioProcessorService.readAudioData();
            STFTUtil::ComputeSTFT(context.spectrogramRepository.audioData,
                                  context.spectrogramRepository.actualWindowSize,
                                  context.spectrogramRepository.actualHopSize,
                                  context.spectrogramRepository.minMagnitude);
        }
    }

    std::string computeMemorySize(std::size_t numInts) {
        constexpr std::size_t bytesPerInt = 4;
        constexpr double bytesPerKB = 1024.0;
        constexpr double bytesPerMB = 1024.0 * 1024.0;

        std::size_t totalBytes = numInts * bytesPerInt;
        double totalKB = totalBytes / bytesPerKB;
        double totalMB = totalBytes / bytesPerMB;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        if (totalBytes >= 1)
            oss << "\n" <<  totalBytes << " B\n";
        if (totalKB >= 1.0)
            oss << totalKB << " KB\n";
        if (totalMB >= 1.0)
            oss << totalMB << " MB";

        return oss.str();
    }

    void VoxelProcessorService::process() {
        refreshData();
        SparseVoxelOctreeBuilder *builder = nullptr;

        if (context.globalRepository.isShowStaticCurve) {
            if (context.globalRepository.selectedCurve > -1) {
                std::unique_ptr<AbstractCurve> &curve = context
                        .globalRepository
                        .curves[context.globalRepository.selectedCurve];
                if (context.globalRepository.useNyquistForTreeDepth) {
                    context.globalRepository.actualTreeDepth = estimateMinimumDepth(*curve.get());
                }
                builder = new SparseVoxelOctreeBuilder(
                    WORLD_VOXEL_SCALE, context.globalRepository.actualTreeDepth);

                MaxAxis maxAxis = curve->addVoxels(*builder);
                context.globalRepository.maxXAxis = maxAxis.x + 1;
                context.globalRepository.maxYAxis = maxAxis.y + 1;
                context.globalRepository.maxZAxis = maxAxis.z + 1;
            }
        } else {
            if (context.globalRepository.useNyquistForTreeDepth) {
                context.globalRepository.actualTreeDepth = estimateSpectrogramOctreeDepth();
            }
            float fScale = static_cast<float>(context.globalRepository.actualTreeDepth);

            builder = new SparseVoxelOctreeBuilder(WORLD_VOXEL_SCALE, static_cast<int>(fScale));
            processAudioInfo(fScale, *builder);
        }

        context.globalRepository.maxXAxis = std::min(
            static_cast<unsigned int>(context.globalRepository.maxXAxis),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 2));
        context.globalRepository.maxYAxis = std::min(
            static_cast<unsigned int>(context.globalRepository.maxYAxis),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 2));
        context.globalRepository.maxZAxis = std::min(
            static_cast<unsigned int>(context.globalRepository.maxZAxis),
            static_cast<unsigned int>(WORLD_VOXEL_SCALE / 2));

        context.cameraService.updateCameraTarget();

        if (builder != nullptr) {
            auto voxels = builder->buildBuffer();
            context.coreBuffers.svoData->update(voxels.data());
            context.globalRepository.voxelCount = builder->getVoxelQuantity();
            context.globalRepository.memorySize = computeMemorySize(builder->getVoxelQuantity());
            builder->dispose();
            delete builder;
        }
    }
} // Metal
