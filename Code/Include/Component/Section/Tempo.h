#pragma once

#include <nierika_dsp/nierika_dsp.h>

namespace nierika::gui::section
{

class Tempo final : public Section
{
public:
    Tempo(const std::string& identifier, ndsp::ParameterManager& parameterManager);
    ~Tempo() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    element::BPMSelector _selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tempo)
};

}
