#pragma once

#include <juce_core/juce_core.h>
#include <vector>
#include <deque>

namespace dsp
{

/**
 * \class OBTAINBeatTracker
 *
 * A JUCE-compatible audio processor that implements the OBTAIN
 * real-time beat tracking algorithm.  This class expects a single
 * channel (mono) or multi-channel signal; the code below uses only
 * the left channel for simplicity, but you can adapt as needed.
 *
 * Main steps:
 *  1) Read audio frames, overlap them at HopSize=128 with FFTSize=1024
 *  2) Compute Onset Strength Signal (OSS) via Flux, smoothing, etc.
 *  3) Estimate Tempo
 *  4) Compute Cumulative Beat Strength Signal (CBSS)
 *  5) Detect periodic peaks in CBSS in a real-time manner
 */
class OBTAINBeatTracker
{
public:
    OBTAINBeatTracker() = default;
    ~OBTAINBeatTracker() = default;

    void prepare(double sampleRate, int fftSize = 1024, int hopSize = 128);
    
    void processAudioBlock(const float* input, int numSample);

    //=======================================================================
    // Accessors
    //=======================================================================
    /**
     * \return The sample index of the most recently detected beat, or -1 if none.
     */
    [[nodiscard]] juce::int64 getLastBeatSample() const { return lastBeatSample; }
    
    [[nodiscard]] double getBPM() const { return stableTempoBPM; }

private:

    //=======================================================================
    // 1) OSS Creation
    //=======================================================================
    /**
     * \brief Compute raw flux from the input frame:
     *        - FFT
     *        - Magnitude & (optional) threshold
     *        - Log compression
     *        - Flux calculation vs. previous log spectrum
     */
    double computeFluxOSS(const std::vector<float>& frame);

    /**
     * \brief Smooth the flux by convolving with a length-15 Hamming window
     *        to produce the final Onset Strength Signal (OSS).
     */
    double smoothFlux(double rawFluxVal);

    //=======================================================================
    // 2) Tempo Estimation
    //=======================================================================
    /**
     * \brief Estimate the current tempo in BPM by:
     *        - Autocorrelating the last N=256 OSS frames
     *        - Finding local peaks that map to BPM in [60..200]
     *        - Cross-correlating with a pulse train
     *        - Returning the best BPM
     */
    double estimateTempo();

    /**
     * \brief Refine tempo changes using short "tempoHistory" memory:
     *        - If new tempo is more than ~5 BPM away, wait ~1s before adopting
     *        - Avoid short-term harmonic misdetections (double/half-time).
     */
    void refineTempo(double newTempo);

    //=======================================================================
    // 3) Cumulative Beat Strength Signal (CBSS)
    //=======================================================================
    /**
     * \brief Compute CBSS for the current frame:
     *        CBSS[n] = (1 - alpha)*OSS[n] + alpha*Phi[n].
     */
    double computeCBSS(double currentOSS);

    /**
     * \brief Compute Phi[n] = max_{v in [-tau_b/2..tau_b/2]} W[v]*CBSS[n+v],
     *        approximating eq. (4)–(5) in the paper with a discrete log-Gaussian.
     */
    double computePhi(int indexN);

    //=======================================================================
    // 4) Parallel Peak Detection
    //=======================================================================
    /**
     * \brief Runs two systems:
     *        - Main system: local maxima ignoring beat period
     *        - Second system: cross-correlation with pulse train of period
     *                        "currentBeatPeriodFrames"
     *        If the second system consistently yields higher peaks, we correct.
     * \return True if a beat is detected in this frame
     */
    bool detectBeat(double cbssVal);

    //=======================================================================
    // Constants & Configuration
    //=======================================================================
    static constexpr int FluxFilterLength  = 15;    ///< Hamming filter for flux smoothing
    static constexpr int OSSBufferLength   = 256;   ///< For storing OSS frames for tempo
    static constexpr int CBSSBufferSize    = 2048;  ///< Ring buffer for CBSS

    //=======================================================================
    // Configuration Parameters
    //=======================================================================
    double currentSampleRate    = 44100.0;
    int FFTSize                 = 1024;  ///< Analysis window size
    int HopSize                 = 128;   ///< Overlap = FFTSize - HopSize

    // Frame assembly
    std::vector<float> frameBuffer;
    int frameWritePos = 0;

    //=======================================================================
    // OSS / Flux Variables
    //=======================================================================
    std::vector<double> previousLogSpec;  ///< Last frame's log spectrum
    std::deque<double> fluxFifo;         ///< FIFO of recent flux values for smoothing
    std::vector<double> hammingWindow;   ///< Hamming kernel for flux smoothing

    //=======================================================================
    // Tempo Estimation Variables
    //=======================================================================
    double estimatedTempoBPM    = 120.0; ///< Raw new tempo from estimateTempo()
    double stableTempoBPM       = 120.0; ///< The refined stable tempo
    int   tempoChangeCountdown  = 0;     ///< Counting frames before finalizing a big tempo jump
    std::deque<double> tempoHistory;    ///< Short memory of tempo for smoothing

    // Store up to 256 OSS frames for cross-correlation approach
    std::vector<double> ossHistory;
    int ossWritePos = 0;
    int ossCount    = 0;

    //=======================================================================
    // CBSS Variables
    //=======================================================================
    double alpha = 0.8;                 ///< Weight factor in eq.(6)
    std::vector<double> cbssValues;     ///< ring buffer for CBSS
    int cbssIndex = 0;                  ///< write index into cbssValues

    int currentBeatPeriodFrames = 32;   ///< integer frames per beat, derived from stableTempoBPM

    //=======================================================================
    // Beat Detection Variables
    //=======================================================================
    // If a beat is detected, store the sample position here
    juce::int64 lastBeatSample = -1;
    // Keep track of how many total samples have passed
    juce::int64 totalSamplesProcessed = 0;

    // "Main" peaks vs "Second" system peaks
    std::vector<double> mainPeaks;
    std::vector<double> secondPeaks;
    
    // example bool if needed
    bool lastFrameWasPeak = false;

    //=======================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OBTAINBeatTracker)
};

}
