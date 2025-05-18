#include "OBTAINBeatTracker.h"

#include "FluxCalculator.h"
#include "FFT.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>

namespace dsp
{

// A small helper to produce a Hamming window of specified length
static std::vector<double> makeHammingWindow(int length)
{
    std::vector<double> w(static_cast<std::size_t>(length));
    constexpr double twoPi = juce::MathConstants<double>::twoPi;
    for (std::size_t i = 0; i < static_cast<std::size_t>(length); ++i)
        w[i] = 0.54 - 0.46 * std::cos(twoPi * static_cast<double>(i) / (length - 1.0));
    return w;
}

//--------------------------------------------------------------
// Called by JUCE when audio starts
//--------------------------------------------------------------
void OBTAINBeatTracker::prepare(double sampleRate, int fftSize, int hopSize)
{
    currentSampleRate = sampleRate;
    FFTSize = fftSize;
    HopSize = hopSize;

    frameBuffer.resize(static_cast<std::size_t>(FFTSize), 0.0f);
    frameWritePos = 0;

    // Initialize the previous log spectrum to zeros
    previousLogSpec.assign(static_cast<std::size_t>(FFTSize / 2), 0.0);

    // Initialize the ring buffer for the last 256 OSS frames (for tempo)
    ossHistory.clear();
    ossHistory.resize(static_cast<std::size_t>(OSSBufferLength), 0.0);
    ossWritePos = 0;
    ossCount = 0;

    // Build the smoothing window for the flux => final OSS
    hammingWindow = makeHammingWindow(FluxFilterLength);

    // Tempo management
    estimatedTempoBPM = 120.0;
    stableTempoBPM = 120.0;
    tempoHistory.clear();

    // CBSS management
    cbssValues.clear();
    cbssValues.resize(CBSSBufferSize, 0.0);
    cbssIndex = 0;
    lastBeatSample = -1;
    totalSamplesProcessed = 0;

    // Peak detection states
    mainPeaks.clear();
    secondPeaks.clear();
    lastFrameWasPeak = false;

    // We'll default the beat period in frames to ~ (60.0 / 120.0) * (sampleRate / HopSize).
    // That is samples per beat => frames per beat => used in second system's cross-corr
    double framesPerBeat = (60.0 / estimatedTempoBPM) * (sampleRate / HopSize);
    currentBeatPeriodFrames = (int) std::round(framesPerBeat);
}

//--------------------------------------------------------------
void OBTAINBeatTracker::processAudioBlock(const float* input, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        // Overlap-add approach: accumulate until we have FFTSize samples
        frameBuffer[static_cast<std::size_t>(frameWritePos++)] = input[i];

        if (frameWritePos >= FFTSize)
        {
            // 1) Compute the flux-based Onset Strength for this new frame
            double rawFluxVal = computeFluxOSS(frameBuffer);

            // 2) Filter the flux to obtain final OSS (Hamming smoothing, eq.(3) in the paper)
            double finalOSS = smoothFlux(rawFluxVal);

            // 3) Store that OSS in a ring buffer for tempo analysis
            ossHistory[static_cast<std::size_t>(ossWritePos)] = finalOSS;
            ossWritePos = (ossWritePos + 1) % OSSBufferLength;
            if (ossCount < OSSBufferLength)
                ossCount++;

            // 4) Perform tempo estimation (when enough frames are gathered)
            //    The paper uses ~3 seconds => with ~43 Hz frames, that is ~129 frames.
            if (ossCount >= 128) // or some suitable threshold
            {
                double newTempo = estimateTempo();
                refineTempo(newTempo); // apply "history-based" smoothing from paper
            }

            // 5) With the (potentially updated) stable tempo, do CBSS recursion
            double cbssVal = computeCBSS(finalOSS);

            // 6) Detect peaks in CBSS (two parallel systems).
            bool detectedBeat = detectBeat(cbssVal);

            if (detectedBeat)
            {
                // Approximate the sample position of the beat as current block index
                // offset by i in this buffer:
                lastBeatSample = totalSamplesProcessed + i;
                // TODO: create click here or update AudioPlayerHead from the AudioProcessor ?
                juce::Logger::writeToLog("** Beat Detected at sample " + juce::String(lastBeatSample));
            }

            // Overlap shift for next frame
            const int shiftSize = FFTSize - HopSize;
            for (int j = 0; j < shiftSize; ++j)
                frameBuffer[static_cast<std::size_t>(j)] = frameBuffer[static_cast<std::size_t>(j + HopSize)];
            frameWritePos = shiftSize;
        }
    }
    totalSamplesProcessed += numSamples;
}

//==============================================================
// ==========  STAGE 1:  OSS CREATION  =========================
//==============================================================
double OBTAINBeatTracker::computeFluxOSS(const std::vector<float>& frame)
{
    // 1) Build real and imag buffer for FFT
    std::vector<double> re(static_cast<std::size_t>(FFTSize)), im(static_cast<std::size_t>(FFTSize), 0.0);
    for (std::size_t i = 0; i < static_cast<std::size_t>(FFTSize); ++i)
        re[i] = static_cast<double>(frame[i]);

    // 1a) Optional: apply a window to the frame (e.g., Hamming)
    // ...
    // for (int i = 0; i < FFTSize; ++i) re[i] *= <some Hamming factor>;

    // 2) Forward FFT
    calculations::performFFT(re, im, false);

    // 3) Convert to magnitude. We only need half the spectrum (0..FFTSize/2-1)
    const auto halfN = static_cast<std::size_t>(FFTSize / 2);
    std::vector<double> mag(halfN, 0.0);
    for (std::size_t i = 0; i < halfN; ++i)
    {
        const double rr = re[i];
        const double ii = im[i];
        mag[i] = std::sqrt(rr*rr + ii*ii);
    }

    // 4) Threshold magnitudes below some dB level => 0
    //    e.g. 74 dB range => mag < (10^(-74/20)) * max => ...
    //    We'll do a simpler approach, ignoring it for brevity or:
    // double maxVal = *std::max_element(mag.begin(), mag.end());
    // double threshold = maxVal * pow(10.0, -74.0/20.0);
    // for (auto &m : mag) { if (m < threshold) m = 0.0; }

    // 5) Log-compress => eq.(1) in the paper
    std::vector<double> logSpec = calculations::logCompress(mag, 10.0);

    // 6) Compute flux => eq.(2), comparing with previous log spectrum
    double fluxVal = calculations::computeFlux(previousLogSpec, logSpec);

    // Update for next time
    previousLogSpec = logSpec;

    return fluxVal;
}

/** We keep a small queue (or buffer) of recent flux values (length 15)
    and convolve with a Hamming window to approximate eq. (3) from the paper. */
double OBTAINBeatTracker::smoothFlux(double rawFluxVal)
{
    // push the raw flux into a small ring buffer
    fluxFifo.push_back(rawFluxVal);
    if (static_cast<int>(fluxFifo.size()) > FluxFilterLength)
        fluxFifo.pop_front();

    // If we don't yet have enough data, just return the raw flux
    if (static_cast<int>(fluxFifo.size()) < FluxFilterLength)
        return rawFluxVal;

    // Convolve with the length=15 Hamming
    double sumVal = 0.0;
    for (std::size_t i = 0; i < static_cast<std::size_t>(FluxFilterLength); ++i)
        sumVal += fluxFifo[i] * hammingWindow[i];

    // You can scale by sum of window if you want an exact match
    double norm = 0.0;
    for (const auto x : hammingWindow) norm += x;
    const double result = sumVal / norm;

    return result;
}

//==============================================================
// ==========  STAGE 2:  TEMPO ESTIMATION  =====================
//==============================================================

/**
 * \brief Estimate the current tempo using the last 256 OSS frames.
 *        Based on the cross-correlation approach from [11] in the paper.
 */
double OBTAINBeatTracker::estimateTempo()
{
    // We'll do a simplified version:
    //  1) Autocorrelate the last 256 OSS frames
    //  2) "Enhance" the autocorrelation
    //  3) Find top 10 peaks that respect minBPM=60, maxBPM=200
    //  4) For each candidate, cross-correlate with a pulse train
    //  5) Return best candidate's BPM

    // Gather up to 256 frames from ossHistory, starting from (ossWritePos - 1) going backward
    const int length = std::min(OSSBufferLength, ossCount);
    std::vector<double> windowedOSS(static_cast<std::size_t>(length));
    // fill in from oldest to newest or vice versa
    // We'll choose newest at the end so the indexing is consistent
    for (int i = 0; i < length; ++i)
    {
        // index from end
        const int idx = (ossWritePos - length + i + OSSBufferLength) % OSSBufferLength;
        windowedOSS[static_cast<std::size_t>(i)] = ossHistory[static_cast<std::size_t>(idx)];
    }

    // 1) Autocorrelation
    std::vector<double> acf(static_cast<std::size_t>(length), 0.0);
    for (int lag = 0; lag < length; ++lag)
    {
        double sum = 0.0;
        for (int t = 0; t < length - lag; ++t)
            sum += windowedOSS[static_cast<std::size_t>(t)] * windowedOSS[static_cast<std::size_t>(t + lag)];
        acf[static_cast<std::size_t>(lag)] = sum;
    }

    // 2) (Optional) "Enhance" harmonics in acf
    //    E.g. you could do a difference of median. We'll omit for brevity.

    // 3) Find candidate peaks in acf that map to BPM in [minBPM, maxBPM]
    const double framesPerSec = (currentSampleRate / (double)HopSize);
    // The lag corresponds to a BPM => BPM = 60 / (lag/framesPerSec)
    // => lag = 60 * framesPerSec / BPM
    // We'll store them in a vector of (acfVal, BPM).
    std::vector<std::pair<double,double>> candidates;
    for (int lag = 1; lag < length; ++lag)
    {
        constexpr double maxBPM = 200.0;
        // BPM from this lag
        double bpm = 60.0 * framesPerSec / (double)lag;
        if (constexpr double minBPM = 60.0; bpm >= minBPM && bpm <= maxBPM)
        {
            // Check if this is a local peak
            double val = acf[static_cast<std::size_t>(lag)];
            // We'll do a small neighborhood check
            if (lag > 0 && lag < length - 1)
            {
                if (acf[static_cast<std::size_t>(lag)] > acf[static_cast<std::size_t>(lag - 1)] && acf[static_cast<std::size_t>(lag)] > acf[static_cast<std::size_t>(lag + 1)])
                {
                    // It's a local peak
                    candidates.emplace_back(val, bpm);
                }
            }
        }
    }

    // If no candidates found, fallback
    if (candidates.empty())
        return estimatedTempoBPM;

    // Sort by ACF strength descending
    std::sort(candidates.begin(), candidates.end(),
              [](auto& a, auto& b){ return a.first > b.first; });

    // Keep top 10
    if (candidates.size() > 10)
        candidates.resize(10);

    // 4) For each candidate BPM, cross-correlate with a pulse train (like [11])
    //    We'll pick the best score.
    //    The pulse train has period = framesPerSec * (60/BPM) in frames.
    double bestScore = -1e9;
    double bestBPM = estimatedTempoBPM;

    for (auto& c : candidates)
    {
        const double candBPM = c.second;
        const double candPeriod = framesPerSec * (60.0 / candBPM);
        // Cross-correlation with pulse train
        // Shift the pulse train for all possible phases to find maximum
        // We'll define a small step for phase in [0, candPeriod)
        double localBest = -1e9;
        for (int phase = 0; phase < static_cast<int>(candPeriod); ++phase)
        {
            double sum = 0.0;
            // Construct pulses at indices: phase + k*candPeriod
            for (int t = phase; t < length; t += static_cast<int>(std::round(candPeriod)))
                sum += windowedOSS[static_cast<std::size_t>(t)];
            if (sum > localBest)
                localBest = sum;
        }
        // Evaluate variance, if you want more advanced scoring => left out for brevity
        if (localBest > bestScore)
        {
            bestScore = localBest;
            bestBPM = candBPM;
        }
    }

    return bestBPM;
}

/**
 * \brief Applies the "history-based smoothing" from the paper:
 *        - if new tempo differs from stable tempo > 5 BPM, ignore it unless it persists ~1s
 *        - keep a short ring buffer of tempo
 */
void OBTAINBeatTracker::refineTempo(double newTempo)
{
    // Add newTempo to ring buffer of the last ~7 seconds
    // => numberOfFrames = 7 * framesPerSec => ~7 * 43 = ~301 frames if you want
    // We'll do something simpler:
    tempoHistory.push_back(newTempo);
    while (static_cast<int>(tempoHistory.size()) > 40) // store ~40 frames worth
        tempoHistory.pop_front();

    double meanTempo = 0.0;
    for (const double t : tempoHistory)
        meanTempo += t;
    meanTempo /= (double) tempoHistory.size();

    (void) meanTempo;

    // If new tempo is far from stable tempo, check if it's a "harmonic" or an outlier
    if (std::fabs(newTempo - stableTempoBPM) > 5.0)
    {
        // e.g. check if newTempo is ~2x or 0.5x old tempo, to see if it's a harmonic
        if (const double ratio = (newTempo / stableTempoBPM); ratio > 1.9 && ratio < 2.1)
        {
            // It's basically double-time -> ignore if you want, or adopt if it persists
        }
        else if (ratio > 0.45 && ratio < 0.55)
        {
            // It's basically half-time
        }
        else
        {
            // not a simple harmonic => wait 1 second => i.e. ~43 frames
            tempoChangeCountdown++;
            if (tempoChangeCountdown > 43)
            {
                stableTempoBPM = newTempo;
                tempoChangeCountdown = 0;
            }
        }
    }
    else
    {
        // adopt new tempo quickly
        stableTempoBPM = newTempo;
        tempoChangeCountdown = 0;
    }

    // update the integer beat period in frames
    const double framesPerSec = currentSampleRate / static_cast<double>(HopSize);
    const double framesPerBeat = (60.0 / stableTempoBPM) * framesPerSec;
    currentBeatPeriodFrames = std::max(1, static_cast<int>(std::round(framesPerBeat)));
}

//==============================================================
// ==========  STAGE 3:  CBSS RECURSION  =======================
//==============================================================

/**
 * \brief Compute the new CBSS value (eq. (6) from the paper) for the current frame.
 *
 *   CBSS[n] = (1-alpha)*OSS[n] + alpha*Phi[n],
 *   where Phi[n] = max_{v in [-tau_b/2, +tau_b/2]} W[v]*CBSS[n+v].
 *   We'll approximate by looking up the last "beatPeriodFrames" around cbssIndex,
 *   applying a log-Gaussian weighting, and taking a max.
 */
double OBTAINBeatTracker::computeCBSS(double currentOSS)
{
    // Grow our cbss buffer
    // We'll store the new CBSS at cbssIndex
    // Then increment. We'll do a ring buffer approach.
    const double phiVal = computePhi(cbssIndex);
    const double newCBSS = (1.0 - alpha)*currentOSS + alpha*phiVal;

    cbssValues[static_cast<std::size_t>(cbssIndex)] = newCBSS;
    cbssIndex = (cbssIndex + 1) % CBSSBufferSize;

    return newCBSS;
}

/**
 * \brief Compute Phi[n] = max_{v around -beatPeriod/2 ... +beatPeriod/2} of W[v]*CBSS[n+v].
 *        We'll define a discrete log-Gaussian window around that range.
 */
double OBTAINBeatTracker::computePhi(int indexN)
{
    // We'll define v in [-halfPeriod, +halfPeriod], discrete steps,
    // then apply W[v], pick max of W[v]*CBSS[indexN + v].
    // Because we're ring buffering, we'll mod to find the right index.

    // halfPeriod frames
    int halfP = currentBeatPeriodFrames / 2;
    if (halfP < 1) halfP = 1;

    // Build a log-Gaussian weighting
    // The paper: W[v] = exp( - (eta*log(-v/tau_b))^2 / 2 ), for v < 0, with tau_b>0
    // We'll do a symmetrical version. We'll define a small eta or ~1.0

    double maxVal = 0.0;
    // We only look behind indexN (like eq. 4 in the paper).
    // For simplicity, we’ll look from -halfP to + halfP, skipping v=0 to avoid log(0).
    for (int v = -halfP; v <= halfP; ++v)
    {
        constexpr double eta = 1.0;
        if (v == 0)
            continue; // skip or handle carefully

        const double ratio = -(double)v / (double)currentBeatPeriodFrames;
        // The minus sign: so that negative v => ratio>0
        if (ratio <= 0.0)
            continue;

        const double exponent = -0.5 * (eta * std::log(ratio)) * (eta * std::log(ratio));
        const double wv = std::exp(exponent);

        // ring buffer indexing
        const int idx = (indexN + v + CBSSBufferSize) % CBSSBufferSize;
        if (const double val = wv * cbssValues[static_cast<std::size_t>(idx)]; val > maxVal)
            maxVal = val;
    }
    return maxVal;
}

//==============================================================
// ==========  STAGE 4:  PARALLEL PEAK DETECTION  ==============
//==============================================================
/**
 * \brief We detect a beat if:
 *   1) The "main system" sees a local max in a short window near the expected beat location
 *   2) The "second system" cross-correlates the CBSS with an impulse train at the known period
 *      and if it yields a systematically higher average, we correct the main system's detection
 */
bool OBTAINBeatTracker::detectBeat(double cbssVal)
{
    bool mainDetected = false;
    bool secondDetected = false;

    const int currentIndex = (cbssIndex - 1 + CBSSBufferSize) % CBSSBufferSize;
    // we just wrote the new CBSS at cbssIndex-1

    // ---- MAIN SYSTEM (ignores beat period; checks local maxima over small window) ---
    // e.g. look if CBSS is bigger than neighbors.
    // Because we do 1 sample per frame, the "neighbors" are the last 2 frames or so.
    if (const double prevVal = cbssValues[static_cast<std::size_t>((currentIndex - 1 + CBSSBufferSize) % CBSSBufferSize)]; cbssVal > prevVal && cbssVal > 0.1) // a threshold
    {
        mainDetected = true;
    }

    // We'll store main system peaks in mainPeaks
    if (mainDetected)
    {
        mainPeaks.push_back( (double) cbssVal );
        if (mainPeaks.size() > 50)
            mainPeaks.erase(mainPeaks.begin());
    }

    // ---- SECOND SYSTEM (use cross-correlation with pulse train of period currentBeatPeriodFrames)
    // We do it every half of the beat period, i.e. if (framesSinceLastSecondSystem >= currentBeatPeriodFrames/2)
    static int framesSinceSecSystem = 0;
    framesSinceSecSystem++;
    if (framesSinceSecSystem >= currentBeatPeriodFrames/2)
    {
        framesSinceSecSystem = 0;
        // Cross-correlation with a pulse train. We take a small window behind currentIndex
        // e.g. 2-3 beat periods. Then shift the pulse train for best alignment.
        double bestCorr = -1e9;
        for (int shift = 0; shift < currentBeatPeriodFrames; ++shift)
        {
            double sumVal = 0.0;
            for (int p = shift; p < 3 * currentBeatPeriodFrames; p += currentBeatPeriodFrames)
            {
                int idx = (currentIndex - p + CBSSBufferSize) % CBSSBufferSize;
                sumVal += cbssValues[static_cast<std::size_t>(idx)];
            }
            if (sumVal > bestCorr)
                bestCorr = sumVal;
        }
        // We'll define a quick threshold that if bestCorr is bigger than X => detect
        if (bestCorr > 2.0) // somewhat arbitrary
        {
            secondDetected = true;
            secondPeaks.push_back(cbssVal);
            if (secondPeaks.size() > 50)
                secondPeaks.erase(secondPeaks.begin());
        }
    }

    // Correction: If the second system's average is bigger than the main system's average => correct
    double avgMain = 0.0;
    if (!mainPeaks.empty())
        avgMain = std::accumulate(mainPeaks.begin(), mainPeaks.end(), 0.0) / static_cast<double>(mainPeaks.size());
    double avgSecond = 0.0;
    if (!secondPeaks.empty())
        avgSecond = std::accumulate(secondPeaks.begin(), secondPeaks.end(), 0.0) / static_cast<double>(secondPeaks.size());

    if (avgSecond > avgMain + 0.2)
    {
        // The second system is more confident => we adopt that peak series
        // This is a simplistic “correction.”  A real approach might forcibly realign
        // the “main system’s last beat index” to the second system’s last detected peak index, etc.
        mainDetected = secondDetected;
    }

    // Return whether a beat was ultimately detected
    return (mainDetected || secondDetected);
}


}
