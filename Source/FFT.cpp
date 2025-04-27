#include "FFT.h"

namespace dsp::calculations
{

    void performFFT(std::vector<double>& dataRe, std::vector<double>& dataIm, bool inverseFFT)
    {
        // Assume dataRe.size() == dataIm.size() == N, a power of 2
        const size_t N = dataRe.size();
        if (N == 0) return;

        // 1) Bit-reversal reorder
        //    We'll do a standard iterative bit-reversal
        size_t j = 0;
        for (size_t i = 0; i < N; ++i)
        {
            if (i < j)
            {
                std::swap(dataRe[i], dataRe[j]);
                std::swap(dataIm[i], dataIm[j]);
            }
            size_t m = N >> 1;
            while (m > 0 && j >= m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }

        // 2) Iterative Danielson–Lanczos
        for (size_t step = 1; step < N; step <<= 1)
        {
            const size_t jump = step << 1;
            // pre-calculate angle
            double theta = (inverseFFT ?  ( M_PI / (double)step )
                                       : -( M_PI / (double)step ));
            double wtemp = std::sin(0.5 * theta);
            double wpr   = -2.0 * wtemp * wtemp;
            double wpi   = std::sin(theta);
            double wr    = 1.0;
            double wi    = 0.0;

            for (size_t group = 0; group < step; ++group)
            {
                for (size_t pair = group; pair < N; pair += jump)
                {
                    size_t match = pair + step;
                    double tempr = wr * dataRe[match] - wi * dataIm[match];
                    double tempi = wr * dataIm[match] + wi * dataRe[match];

                    dataRe[match] = dataRe[pair] - tempr;
                    dataIm[match] = dataIm[pair] - tempi;
                    dataRe[pair] += tempr;
                    dataIm[pair] += tempi;
                }
                // Update wr, wi
                double wtempWr = wr;
                wr = wtempWr * wpr - wi * wpi + wr;
                wi = wi * wpr + wtempWr * wpi + wi;
            }
        }

        // 3) If inverse FFT, scale results
        if (inverseFFT)
        {
            for (size_t i = 0; i < N; ++i)
            {
                dataRe[i] /= (double)N;
                dataIm[i] /= (double)N;
            }
        }
    }

}
