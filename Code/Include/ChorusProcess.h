#pragma once

#include <juce_dsp/juce_dsp.h>
#include "AudioProcess.h"

namespace nierika::dsp
{

class ChorusProcess final : public AudioProcess
{
public:
    ChorusProcess();

    [[nodiscard]] bool isEnabled() const noexcept override;
    void setEnabled(bool isEnabled) noexcept override;
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() noexcept override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override;
    
    void setRate(float value);
    void setDepth(float value);
    void setHPF(float value) const;
    void setLPF(float value) const;
    void setDryWet(float value);

private:
    bool _isEnabled = true;
    double _sampleRate = 44100.0;
    juce::dsp::Chorus<float> _chorusProcess;
    juce::dsp::DryWetMixer<float> _dryWetMixer;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _highPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> _lowPassFilter;
};

}
