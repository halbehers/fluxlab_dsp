#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

namespace nierika::gui::section
{

class DJFilter final : public Section
{
public:
    explicit DJFilter(ndsp::ParameterManager& parameterManager);
    ~DJFilter() override = default;

    void paint(juce::Graphics&) override;

private:
    element::DJFilterDial _value;
    element::Dial _q;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DJFilter)
};

}
