#pragma once

#include <juce_dsp/juce_dsp.h>

namespace nierika::dsp
{

class AudioProcess
{
public:
    virtual ~AudioProcess() = default;

    [[nodiscard]] virtual bool isEnabled() const noexcept = 0;
    virtual void setEnabled(bool newValue) noexcept = 0;
    virtual void prepare(const juce::dsp::ProcessSpec& spec) = 0;
    virtual void reset() noexcept = 0;
    virtual void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept = 0;
    [[nodiscard]] virtual bool shouldProcess() const noexcept
    {
        return isEnabled();
    }
};

}
 
