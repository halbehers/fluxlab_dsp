#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

namespace nierika::gui::section
{

class Delay final : public Section
{
public:
    explicit Delay(ndsp::ParameterManager& parameterManager);
    ~Delay() override = default;

    void paint(juce::Graphics&) override;

private:
    element::TimingDial _timing;
    element::Dial _feedback;
    element::FrequencyDial _highPass;
    element::FrequencyDial _lowPass;
    element::PercentageDial _dryWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Delay)
};

}
