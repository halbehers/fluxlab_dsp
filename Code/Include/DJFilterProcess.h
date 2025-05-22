#pragma once

#include <juce_dsp/juce_dsp.h>
#include "AudioProcess.h"

namespace nierika::dsp
{

class DJFilterProcess final : public AudioProcess
{
public:
    DJFilterProcess();

    [[nodiscard]] bool isEnabled() const noexcept override;
    void setEnabled(bool isEnabled) noexcept override;
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() noexcept override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override;

    void setValue(float value);
    void setQ(float value);

private:
    static constexpr float minCutoffInHz = 20.f;
    static constexpr float maxCutoffInHz = 20000.f;

    double _sampleRate = 44100.0;
    bool _isEnabled = true;
    float _relativeFilterValue = 0.f;
    float _frequency = 0.f;
    float _q = .707f;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _highPassFilterProcess;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _lowPassFilterProcess;
    juce::dsp::Gain<float> _gain {};

    void makeFilter() const;
    static float computeFrequencyFromRelativeValue(float value);
    static float computeQCompensationGain(float resonance);
    static bool isLowPass(float value);
    bool isLowPass() const;
};

}
