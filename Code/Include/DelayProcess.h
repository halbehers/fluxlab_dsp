#pragma once

#include <juce_dsp/juce_dsp.h>
#include <nierika_dsp/include/dsp/Timing.h>
#include "AudioProcess.h"

namespace nierika::dsp
{

class DelayProcess final : public AudioProcess
{
public:
    DelayProcess();

    [[nodiscard]] bool isEnabled() const noexcept override;
    void setEnabled(bool isEnabled) noexcept override;
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() noexcept override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override;
    
    void setTimeInMs(float value);
    void setTiming(Timing::NoteTiming timing);
    void setBPM(double value);
    void setFeedback(float value);
    void setHPF(float value) const;
    void setLPF(float value) const;
    void setDryWet(float value);
    void setSync(bool isSync);

private:
    const float _maxDelayTimeInMs = 2000.f;
    bool _isEnabled = true;
    double _sampleRate = 44100.0;
    double _bpm = 120.0;
    float _feedback = 0.0f;
    bool _isSync = true;
    float _timeInMs = 0.f;
    Timing::NoteTiming _timing = Timing::NoteTiming::NOTE_4;
    juce::dsp::DelayLine<float> _delayProcess;
    juce::dsp::DryWetMixer<float> _dryWetMixer;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _highPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _lowPassFilter;

    float convertMsToSamples(float timeInMs) const;
    void processDelay(const juce::dsp::ProcessContextReplacing<float>& context) noexcept;
};

}
