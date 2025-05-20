#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

namespace nierika::gui::section
{

class Chorus final : public Section
{
public:
    explicit Chorus(ndsp::ParameterManager& parameterManager);
    ~Chorus() override = default;

    void paint(juce::Graphics&) override;

private:
    element::FrequencyDial _rate;
    element::PercentageDial _depth;
    element::FrequencyDial _highPass;
    element::FrequencyDial _lowPass;
    element::PercentageDial _dryWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus)
};

}
