#include "ReverbProcess.h"

namespace nierika::dsp
{

ReverbProcess::ReverbProcess():
    _highPassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, 2000.0, 1.0)),
    _lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 16000.0, 1.0))
{
}

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
    _sampleRate = spec.sampleRate;

    _dryWetMixer.prepare(spec);
    _highPassFilter.prepare(spec);
    _lowPassFilter.prepare(spec);

    reset();
    _reverbProcess.prepare(spec);
}

void ReverbProcess::reset() noexcept
{
    _dryWetMixer.reset();
    _highPassFilter.reset();
    _lowPassFilter.reset();
    _reverbProcess.reset();

    auto parameters = _reverbProcess.getParameters();
    parameters.dryLevel = 0.f;
    parameters.wetLevel = 1.f;
    _reverbProcess.setParameters(parameters);

    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void ReverbProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    _dryWetMixer.pushDrySamples(context.getInputBlock());
    _reverbProcess.process(context);
    _highPassFilter.process(context);
    _lowPassFilter.process(context);
    _dryWetMixer.mixWetSamples(context.getOutputBlock() );
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

void ReverbProcess::setDryWet(float value)
{
    jassert(value >= 0.f && value <= 1.f);

    _dryWetMixer.setWetMixProportion(value);
}

void ReverbProcess::setHPF(float value) const
{
    *_highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, value, 1.0);
}

void ReverbProcess::setLPF(float value) const
{
    *_lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, value, 1.0);
}

}

