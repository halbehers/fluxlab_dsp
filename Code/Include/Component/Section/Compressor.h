#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

namespace nierika::gui::section
{

class Compressor final : public Section
{
public:
    explicit Compressor(ndsp::ParameterManager& parameterManager);
    ~Compressor() override = default;

    void paint(juce::Graphics&) override;

private:
    element::Dial _threshold;
    element::Dial _ratio;
    element::TimeInMsDial _attack;
    element::TimeInMsDial _release;
    element::PercentageDial _dryWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor)
};

}
