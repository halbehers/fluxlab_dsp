#pragma once

#include <JuceHeader.h>

#include "Parameters.h"

namespace nierika::gui::section
{

class Reverb: public Section
{
public:
    Reverb(ndsp::ParameterManager& parameterManager);
    ~Reverb();

    void paint(juce::Graphics&) override;

private:
    element::PercentageDial _size;
    element::PercentageDial _width;
    element::FrequencyDial _highPass;
    element::FrequencyDial _lowPass;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverb)
};

}
