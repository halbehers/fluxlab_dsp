#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

namespace nierika::gui::section
{

class Phaser final : public Section
{
public:
    explicit Phaser(ndsp::ParameterManager& parameterManager);
    ~Phaser() override = default;

    void paint(juce::Graphics&) override;

private:
    element::TimingDial _rate;
    element::PercentageDial _depth;
    element::FrequencyDial _highPass;
    element::FrequencyDial _lowPass;
    element::PercentageDial _dryWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Phaser)
};

}
