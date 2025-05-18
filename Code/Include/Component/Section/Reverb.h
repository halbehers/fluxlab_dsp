#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

namespace nierika::gui::section
{

class Reverb: public Section
{
public:
    explicit Reverb(ndsp::ParameterManager& parameterManager);
    ~Reverb() override = default;

    void paint(juce::Graphics&) override;

private:
    element::PercentageDial _size;
    element::PercentageDial _width;
    element::FrequencyDial _highPass;
    element::FrequencyDial _lowPass;
    element::PercentageDial _dryWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverb)
};

}
