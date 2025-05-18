#pragma once

#include <juce_dsp/juce_dsp.h>
#include "AudioProcess.h"

namespace nierika::dsp
{

class ReverbProcess: public AudioProcess
{
public:
    [[nodiscard]] bool isEnabled() const noexcept override;
    void setEnabled(bool isEnabled) noexcept override;
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() noexcept override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override;
    
    void setSize(float value);
    void setWidth(float value);
    void setHPF(float value);
    void setLPF(float value);

private:
    juce::dsp::Reverb _reverbProcess;
    float _hpf = 0.f;
    float _lpf = 0.f;
};

}
