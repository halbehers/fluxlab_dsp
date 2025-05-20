#pragma once

#include <juce_dsp/juce_dsp.h>
#include "AudioProcess.h"

namespace nierika::dsp
{

class CompressorProcess final : public AudioProcess
{
public:
    CompressorProcess() = default;

    [[nodiscard]] bool isEnabled() const noexcept override;
    void setEnabled(bool isEnabled) noexcept override;
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() noexcept override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override;
    
    void setThreshold(float value);
    void setRatio(float value);
    void setAttack(float value);
    void setRelease(float value);
    void setDryWet(float value);

private:
    bool _isEnabled = true;
    double _sampleRate = 44100.0;
    juce::dsp::Compressor<float> _compressorProcess;
    juce::dsp::DryWetMixer<float> _dryWetMixer;
};

}
