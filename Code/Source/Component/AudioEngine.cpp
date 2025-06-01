#include "AudioEngine.h"

#include <algorithm>

namespace nierika::dsp
{

bool AudioEngine::isEnabled() const noexcept
{
    return _isEnabled;
}

void AudioEngine::setEnabled(bool isEnabled) noexcept
{
    _isEnabled = isEnabled;
}

void AudioEngine::prepare(const juce::dsp::ProcessSpec& spec)
{
    _dryWetMixer.prepare(spec);
    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);

    for (const std::unique_ptr<Process>& process : _processes)
        process->audioProcess.prepare(spec);
}

void AudioEngine::reset() noexcept
{
    _dryWetMixer.reset();
    _dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);

    for (const std::unique_ptr<Process>& process : _processes)
        process->audioProcess.reset();
}

void AudioEngine::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    if (!shouldProcess()) return;

    _dryWetMixer.pushDrySamples(context.getInputBlock());

    for (const std::unique_ptr<Process>& process : _processes)
    {
        if (!process->audioProcess.shouldProcess()) continue;

        process->audioProcess.process(context);
    }

    _dryWetMixer.mixWetSamples(context.getOutputBlock());
}

void AudioEngine::addProcess(const std::string& identifier, AudioProcess& process)
{
    _processes.push_back(std::make_unique<Process>(identifier, process));
}

void AudioEngine::moveProcess(const std::string& identifier, int newPosition)
{
    if (newPosition < 0 || newPosition >= static_cast<int>(_processes.size()))
    {
        nutils::AppLogger::error("Position out of range", "AudioEngine::moveProcess");
        throw std::out_of_range("AudioEngine::moveProcess: position out of range");
    }

    if (const int position = getPosition(identifier); position != -1)
    {
        std::iter_swap(_processes.begin() + position, _processes.begin() + newPosition);
    }
    else
    {
        nutils::AppLogger::error("Identifier " + identifier + " not found", "AudioEngine::moveProcess");
        throw std::out_of_range("AudioEngine::moveProcess: Identifier " + identifier + " not found");
    }
}

void AudioEngine::swapProcesses(const std::string& firstProcessID, const std::string& secondProcessID)
{
    const int secondProcessPosition = getPosition(secondProcessID);

    moveProcess(firstProcessID, secondProcessPosition);
}

void AudioEngine::removeProcess(const std::string& identifier)
{
    auto it = std::remove_if(_processes.begin(), _processes.end(), [&identifier](const std::unique_ptr<Process>& process) {
        return process->identifier == identifier;
    });

    if (it == _processes.end())
    {
        nutils::AppLogger::error("Identifier " + identifier + " not found", "AudioEngine::removeProcess");
        throw std::out_of_range("AudioEngine::removeProcess: Identifier " + identifier + " not found");
    }
}

int AudioEngine::count() const
{
    return static_cast<int>(_processes.size());
}

void AudioEngine::setDryWet(float value)
{
    jassert(value >= 0.f && value <= 1.f);

    _dryWetMixer.setWetMixProportion(value);
}

int AudioEngine::getPosition(const std::string& identifier) const
{
    for (std::size_t i = 0; i < _processes.size(); ++i)
    {
        if (_processes[i]->identifier == identifier)
            return static_cast<int>(i);
    }
    return -1;
}

std::string AudioEngine::getIdentifier(int position) const
{
    if (position < 0 || position >= static_cast<int>(_processes.size()))
        return "";

    return _processes[static_cast<std::size_t>(position)]->identifier;
}

const AudioProcess& AudioEngine::getAudioProcess(int position) const
{
    if (position < 0 || position >= static_cast<int>(_processes.size()))
    {
        nutils::AppLogger::error("Position out of range", "AudioEngine::getAudioProcess");
        throw std::out_of_range("AudioEngine::getAudioProcess: position out of range");
    }

    return _processes[static_cast<std::size_t>(position)]->audioProcess;
}

}

