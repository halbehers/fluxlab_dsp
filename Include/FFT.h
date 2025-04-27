#pragma once

#include <vector>

namespace dsp::calculations
{

    /**
     * \brief Performs an in-place Cooley–Tukey FFT.
     *
     * The data vector must be of size N = 2^m (a power of 2).
     * The real/imag values should be stored in dataRe/dataIm.
     */
    void performFFT(std::vector<double>& dataRe,
                    std::vector<double>& dataIm,
                    bool inverseFFT = false);

}
