#include "ReverbProcess.h"

namespace nierika::dsp
{

bool ReverbProcess::isEnabled() const noexcept
{
    return _reverbProcess.isEnabled();
}

void ReverbProcess::setEnabled(bool isEnabled) noexcept
{
    _reverbProcess.setEnabled(isEnabled);
}

void ReverbProcess::prepare(const juce::dsp::ProcessSpec& spec)
{
    _reverbProcess.prepare(spec);
}

void ReverbProcess::reset() noexcept
{
    _reverbProcess.reset();
}

void ReverbProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    _reverbProcess.process(context);
}

void ReverbProcess::setSize(float value)
{
    auto parameters = _reverbProcess.getParameters();
    parameters.roomSize = value;
    _reverbProcess.setParameters(parameters);
}

void ReverbProcess::setWidth(float value)
{
    auto parameters = _reverbProcess.getParameters();
    parameters.width = value;
    _reverbProcess.setParameters(parameters);
}

void ReverbProcess::setHPF(float value)
{
    _hpf = value;
}

void ReverbProcess::setLPF(float value)
{
    _lpf = value;
}

}

