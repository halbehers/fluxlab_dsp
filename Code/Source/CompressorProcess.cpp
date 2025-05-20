#include "CompressorProcess.h"

namespace nierika::dsp
{

bool CompressorProcess::isEnabled() const noexcept
{
    return _isEnabled;
}

void CompressorProcess::setEnabled(bool isEnabled) noexcept
{
    _isEnabled = isEnabled;
}

void CompressorProcess::prepare(const juce::dsp::ProcessSpec& spec)
{
    _sampleRate = spec.sampleRate;

    _dryWetMixer.prepare(spec);

    reset();
    _compressorProcess.prepare(spec);
}

void CompressorProcess::reset() noexcept
{
    _dryWetMixer.reset();
    _compressorProcess.reset();

    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void CompressorProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    if (!_isEnabled) return;

    _dryWetMixer.pushDrySamples(context.getInputBlock());
    _compressorProcess.process(context);
    _dryWetMixer.mixWetSamples(context.getOutputBlock() );
}

void CompressorProcess::setThreshold(float value)
{
    _compressorProcess.setThreshold(value);
}

void CompressorProcess::setRatio(float value)
{
    _compressorProcess.setRatio(value);
}

void CompressorProcess::setAttack(float value)
{
    _compressorProcess.setAttack(value);
}

void CompressorProcess::setRelease(float value)
{
    _compressorProcess.setRelease(value);
}

void CompressorProcess::setDryWet(float value)
{
    jassert(value >= 0.f && value <= 1.f);

    _dryWetMixer.setWetMixProportion(value);
}

}

