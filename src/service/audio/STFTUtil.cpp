#include "STFTUtil.h"

#define PI 3.14159265358979323846

namespace Metal {
    void STFTUtil::FastFourierTransform(ComplexVector &a) {
        const size_t N = a.size();
        if (N <= 1) return;

        // Divide
        ComplexVector even(N / 2), odd(N / 2);
        for (size_t i = 0; i < N / 2; ++i) {
            even[i] = a[i * 2];
            odd[i] = a[i * 2 + 1];
        }

        // Conquer
        FastFourierTransform(even);
        FastFourierTransform(odd);

        // Combine
        for (size_t k = 0; k < N / 2; ++k) {
            Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
            a[k] = even[k] + t;
            a[k + N / 2] = even[k] - t;
        }
    }

    std::vector<double> STFTUtil::HammingWindow(unsigned int size) {
        std::vector<double> window(size);
        for (int i = 0; i < size; ++i) {
            window[i] = 0.54 - 0.46 * cos(2 * PI * i / (size - 1));
        }
        return window;
    }

    void STFTUtil::ComputeSTFT(AudioDataVector &data, unsigned int windowSize, unsigned int hopSize, float minMagnitude) {
        if (data.data.size() < windowSize) return;

        std::vector<double> window = HammingWindow(windowSize);

        unsigned int maxFrequency = 0;
        double maxMagnitude = 0;
        for (size_t i = 0; i + windowSize <= data.data.size(); i += hopSize) {
            ComplexVector input(windowSize);
            for (size_t j = 0; j < windowSize; ++j)
                input[j] = data.data[i + j].amplitude * window[j];

            FastFourierTransform(input);

            // Calculate magnitudes for first half (real-valued signal)
            std::vector<Frequency> magnitudes;
            for (unsigned int k = 0; k < windowSize / 2; ++k) {
                double magnitude = std::abs(input[k]);
                if (magnitude > minMagnitude) {
                    magnitudes.emplace_back(k + 1, magnitude);
                    maxFrequency = std::max(maxFrequency, k + 1);
                    maxMagnitude = std::max(maxMagnitude, magnitude);
                }
            }

            size_t center = i + windowSize / 2;
            if (center < data.data.size())
                data.data[center].frequencies = std::move(magnitudes);
        }

        data.maxFrequency = maxFrequency;
        data.maxMagnitude = maxMagnitude;
    }
} // Metal
