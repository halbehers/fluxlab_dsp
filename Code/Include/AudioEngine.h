#pragma once

#include <juce_dsp/juce_dsp.h>
#include <nierika_dsp/nierika_dsp.h>

#include "AudioProcess.h"

namespace nierika::dsp
{

class AudioEngine: public AudioProcess
{
public:
    [[nodiscard]] bool isEnabled() const noexcept override;
    void setEnabled(bool isEnabled) noexcept override;
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() noexcept override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override;

    void addProcess(const std::string& identifier, AudioProcess& process);
    void moveProcess(const std::string& identifier, int newPosition);
    void removeProcess(const std::string& identifier);

private:
    struct Process
    {
        std::string identifier;
        AudioProcess& audioProcess;
    };
    bool _isEnabled = true;
    std::vector<std::unique_ptr<Process>> _processes;

    [[nodiscard]] int count() const;
    [[nodiscard]] int getPosition(const std::string& identifier) const;
    [[nodiscard]] std::string getIdentifier(int position) const;
    [[nodiscard]] const AudioProcess& getAudioProcess(int position) const;
};

}
