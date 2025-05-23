#include "DelayProcess.h"

namespace nierika::dsp
{

DelayProcess::DelayProcess():
    _highPassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, 2000.0, 1.0)),
    _lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 16000.0, 1.0))
{
    _delayProcess.setMaximumDelayInSamples(static_cast<int>(_maxDelayTimeInMs / 1000.f * static_cast<float>(_sampleRate)));
}

bool DelayProcess::isEnabled() const noexcept
{
    return _isEnabled;
}

void DelayProcess::setEnabled(bool isEnabled) noexcept
{
    _isEnabled = isEnabled;
}

void DelayProcess::prepare(const juce::dsp::ProcessSpec& spec)
{
    _sampleRate = spec.sampleRate;

    _dryWetMixer.prepare(spec);
    _highPassFilter.prepare(spec);
    _lowPassFilter.prepare(spec);

    reset();
    _delayProcess.prepare(spec);
    _delayProcess.setMaximumDelayInSamples(static_cast<int>(_maxDelayTimeInMs / 1000.f * static_cast<float>(_sampleRate)));
}

void DelayProcess::reset() noexcept
{
    _dryWetMixer.reset();
    _highPassFilter.reset();
    _lowPassFilter.reset();
    _delayProcess.reset();

    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void DelayProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    if (!isEnabled()) return;

    _dryWetMixer.pushDrySamples(context.getInputBlock());
    _delayProcess.process(context);


    for (int sample = 0; sample < context.getInputBlock().getNumSamples(); ++sample)
    {
        const float output = _delayProcess.popSample(channel);
        _delayProcess.pushSample(channel, channelData[sample] + feedback * output);
        context.getOutputBlock()[sample] += output;
    }

    _highPassFilter.process(context);
    _lowPassFilter.process(context);
    _dryWetMixer.mixWetSamples(context.getOutputBlock());
}

void DelayProcess::setTimeInMs(float value)
{
    if (_isSync) return;

    _delayProcess.setDelay(convertMsToSamples(value));
}

void DelayProcess::setTiming(Timing::NoteTiming timing)
{
    if (!_isSync) return;

    _delayProcess.setDelay(convertMsToSamples(Timing::getTimeInMs(_bpm, timing)));
}

void DelayProcess::setFeedback(float value)
{
    _feedback = value;
}

void DelayProcess::setBPM(double value)
{
    _bpm = value;
}

void DelayProcess::setDryWet(float value)
{
    jassert(value >= 0.f && value <= 1.f);

    _dryWetMixer.setWetMixProportion(value);
}

void DelayProcess::setHPF(float value) const
{
    *_highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, value, 1.0);
}

void DelayProcess::setLPF(float value) const
{
    *_lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, value, 1.0);
}

void DelayProcess::setSync(bool isSync)
{
    _isSync = isSync;
}

float DelayProcess::convertMsToSamples(float timeInMs) const
{
    return timeInMs / 1000.f * static_cast<float>(_sampleRate);
}

}

