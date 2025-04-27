#include "FluxCalculator.h"

namespace dsp::calculations
{

    std::vector<double> logCompress(const std::vector<double>& magSpectrum,
                                    double gamma)
    {
        // According to eq. (1) from the paper: log(1 + gamma * |X|) / log(1 + gamma)
        // We do max(0, someValue) checks as needed or clamp below if desired.
        std::vector<double> result(magSpectrum.size());
        double denom = std::log(1.0 + gamma);
        for (size_t i = 0; i < magSpectrum.size(); ++i)
        {
            double val = magSpectrum[i];
            double compressed = std::log(1.0 + gamma * std::fabs(val)) / denom;
            result[i] = compressed;
        }
        return result;
    }

    double computeFlux(const std::vector<double>& prevLogSpectrum,
                       const std::vector<double>& currLogSpectrum)
    {
        // flux[n] = sum_k of max(0, currLogSpectrum[k] - prevLogSpectrum[k])
        // or sum of |curr - prev|+ if you prefer symmetrical difference
        // The paper uses: sum of positive differences only (eq. 2).
        // We'll do eq. (2): sum( |Gamma[n+1,k] - Gamma[n,k]|+ )
        double fluxVal = 0.0;
        const size_t N = std::min(prevLogSpectrum.size(), currLogSpectrum.size());
        for (size_t k = 0; k < N; ++k)
        {
            double diff = currLogSpectrum[k] - prevLogSpectrum[k];
            if (diff > 0.0)
                fluxVal += diff;
        }
        return fluxVal;
    }

}
