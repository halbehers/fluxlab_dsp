#include "ChorusProcess.h"

namespace nierika::dsp
{

ChorusProcess::ChorusProcess():
    _highPassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, 220000.0, 1.0)),
    _lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 15000.0, 1.0))
{
}

bool ChorusProcess::isEnabled() const noexcept
{
    return _isEnabled;
}

void ChorusProcess::setEnabled(bool isEnabled) noexcept
{
    _isEnabled = isEnabled;
}

void ChorusProcess::prepare(const juce::dsp::ProcessSpec& spec)
{
    _sampleRate = spec.sampleRate;

    _dryWetMixer.prepare(spec);
    _highPassFilter.prepare(spec);
    _lowPassFilter.prepare(spec);

    reset();
    _chorusProcess.prepare(spec);
}

void ChorusProcess::reset() noexcept
{
    _dryWetMixer.reset();
    _highPassFilter.reset();
    _lowPassFilter.reset();
    _chorusProcess.reset();

    _chorusProcess.setMix(1.f);
    _chorusProcess.setCentreDelay(7.f);
    _chorusProcess.setFeedback(-.5f);

    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void ChorusProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    if (!_isEnabled) return;

    _dryWetMixer.pushDrySamples(context.getInputBlock());
    _chorusProcess.process(context);
    _highPassFilter.process(context);
    _lowPassFilter.process(context);
    _dryWetMixer.mixWetSamples(context.getOutputBlock());
}

void ChorusProcess::setRate(float value)
{
    _chorusProcess.setRate(value);
}

void ChorusProcess::setDepth(float value)
{
    _chorusProcess.setDepth(value);
}

void ChorusProcess::setDryWet(float value)
{
    jassert(value >= 0.f && value <= 1.f);

    _dryWetMixer.setWetMixProportion(value);
}

void ChorusProcess::setHPF(float value) const
{
    *_highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, value, 1.0);
}

void ChorusProcess::setLPF(float value) const
{
    *_lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, value, 1.0);
}

}

