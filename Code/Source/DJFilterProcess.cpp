#include "DJFilterProcess.h"

#include "nierika_dsp/include/utils/FloatingPointUtils.h"

namespace nierika::dsp
{

DJFilterProcess::DJFilterProcess():
    _highPassFilterProcess(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, 0.0, 1.0)),
    _lowPassFilterProcess(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 22000.0, 1.0))
{
}

bool DJFilterProcess::isEnabled() const noexcept
{
    return _isEnabled;
}

void DJFilterProcess::setEnabled(bool isEnabled) noexcept
{
    _isEnabled = isEnabled;
}

void DJFilterProcess::prepare(const juce::dsp::ProcessSpec& spec)
{
    _sampleRate = spec.sampleRate;
    _highPassFilterProcess.prepare(spec);
    _lowPassFilterProcess.prepare(spec);
    _gain.prepare(spec);
}

void DJFilterProcess::reset() noexcept
{
    _highPassFilterProcess.reset();
    _lowPassFilterProcess.reset();
    _gain.reset();
}

void DJFilterProcess::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    if (!isEnabled() || utils::FloatingPointUtils::isEmpty(_relativeFilterValue)) return;

    (isLowPass() ? _lowPassFilterProcess : _highPassFilterProcess).process(context);
    _gain.process(context);
}

void DJFilterProcess::setValue(float value)
{
    jassert(value >= -1.f && value <= 1.f);

    if (utils::FloatingPointUtils::isEmpty(value)) {
        _relativeFilterValue = 0.f;
        _frequency = 0.f;
    } else {
        _relativeFilterValue = value;
        _frequency = computeFrequencyFromRelativeValue(value);
    }

    makeFilter();
}

void DJFilterProcess::setQ(float value)
{
    _q = value;

    makeFilter();

    _gain.setGainLinear(computeQCompensationGain(value));
}

void DJFilterProcess::makeFilter() const
{
    if (isLowPass()) {
        *_lowPassFilterProcess.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, _frequency, _q);
        *_highPassFilterProcess.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, 0.f, _q);
    } else {
        *_highPassFilterProcess.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(_sampleRate, _frequency, _q);
        *_lowPassFilterProcess.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(_sampleRate, 22000.f, _q);
    }
}

float DJFilterProcess::computeFrequencyFromRelativeValue(float value)
{
    value = std::clamp(value, -1.f, 1.f);

    const float normalized = std::abs(value);

    const float logMin = std::log10(isLowPass(value) ? maxCutoffInHz : minCutoffInHz);
    const float logMax = std::log10(isLowPass(value) ? minCutoffInHz : maxCutoffInHz);
    const float logFreq = logMin + normalized * (logMax - logMin);

    return  std::pow(10.0f, logFreq);;
}

bool DJFilterProcess::isLowPass() const {
    return isLowPass(_relativeFilterValue);
}

bool DJFilterProcess::isLowPass(float value) {
    return value < 0.f;
}

float DJFilterProcess::computeQCompensationGain(float resonance)
{
    resonance = std::clamp(resonance, .1f, 10.f);

    const float logQ = std::log10(resonance);
    const float logQMin = std::log10(0.1f);
    const float logQMax = std::log10(10.0f);
    const float t = 1.0f - (logQ - logQMin) / (logQMax - logQMin);

    return 0.6f + t * (1.0f - 0.6f);
}


}
