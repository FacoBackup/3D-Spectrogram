#ifndef STFTSERVICE_H
#define STFTSERVICE_H
#include "AudioData.h"
#include <vector>
#include <complex>
#include "../../definitions.h"

using Complex = std::complex<double>;
using ComplexVector = std::vector<Complex>;

namespace Metal {
    class STFTUtil {
        // Recursive Cooley-Tukey FFT
        static void FastFourierTransform(ComplexVector &a);

        static std::vector<double> HammingWindow(unsigned int size);

    public:
        static void ComputeSTFT(AudioDataVector &data,
                                unsigned int windowSize = 1024,
                                unsigned int hopSize = DEFAULT_HOP_SIZE);
    };
} // Metal

#endif //STFTSERVICE_H
