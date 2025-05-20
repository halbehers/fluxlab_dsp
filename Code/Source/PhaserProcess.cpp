#include "PhaserProcess.h"

namespace nierika::dsp
{

PhaserProcess::PhaserProcess():
    _highPassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, 220000.0, 1.0)),
    _lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 15000.0, 1.0))
{
}

bool PhaserProcess::isEnabled() const noexcept
{
    return _isEnabled;
}

void PhaserProcess::setEnabled(bool isEnabled) noexcept
{
    _isEnabled = isEnabled;
}

void PhaserProcess::prepare(const juce::dsp::ProcessSpec& spec)
{
    _sampleRate = spec.sampleRate;

    _dryWetMixer.prepare(spec);
    _highPassFilter.prepare(spec);
    _lowPassFilter.prepare(spec);

    reset();
    _phaserProcess.prepare(spec);
}

void PhaserProcess::reset() noexcept
{
    _dryWetMixer.reset();
    _highPassFilter.reset();
    _lowPassFilter.reset();
    _phaserProcess.reset();

    _phaserProcess.setMix(1.f);
    _phaserProcess.setCentreFrequency(10000.f);
    _phaserProcess.setFeedback(-.5f);

    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void PhaserProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    if (!_isEnabled) return;

    _dryWetMixer.pushDrySamples(context.getInputBlock());
    _phaserProcess.process(context);
    _highPassFilter.process(context);
    _lowPassFilter.process(context);
    _dryWetMixer.mixWetSamples(context.getOutputBlock() );
}

void PhaserProcess::setRate(float value)
{
    _phaserProcess.setRate(value);
}

void PhaserProcess::setDepth(float value)
{
    _phaserProcess.setDepth(value);
}

void PhaserProcess::setDryWet(float value)
{
    jassert(value >= 0.f && value <= 1.f);

    _dryWetMixer.setWetMixProportion(value);
}

void PhaserProcess::setHPF(float value) const
{
    *_highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, value, 1.0);
}

void PhaserProcess::setLPF(float value) const
{
    *_lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, value, 1.0);
}

}

