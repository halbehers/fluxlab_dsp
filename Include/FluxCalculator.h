#pragma once

#include <vector>
#include <complex>

namespace dsp::calculations
{
    /**
     * \brief Computes the flux between two adjacent spectra.
     * The flux is often used as an onset detector feature.
     *
     * \param prevLogSpectrum The log-compressed spectrum for the previous frame.
     * \param currLogSpectrum The log-compressed spectrum for the current frame.
     * \return double The flux value (>= 0).
     */
    double computeFlux(const std::vector<double>& prevLogSpectrum,
                       const std::vector<double>& currLogSpectrum);

    /**
     * \brief A helper to convert magnitude spectrum to log-compressed scale.
     *        You may adjust the constant gamma as needed (paper uses ~ e.g. 10 or so).
     *
     * \param magSpectrum The input magnitude spectrum.
     * \param gamma A gamma parameter controlling the log compression strength.
     * \return std::vector<double> The log-compressed spectrum.
     */
    std::vector<double> logCompress(const std::vector<double>& magSpectrum,
                                    double gamma = 10.0);
}
