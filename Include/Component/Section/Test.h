#pragma once

#include <nierika_dsp/nierika_dsp.h>
#include <juce_graphics/juce_graphics.h>

#include "PluginProcessor.h"

namespace component::section
{

class Test : public nui::Section
{
public:
    Test(const std::string& identifier, ndsp::ParameterManager& parameterManager);
    ~Test() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Test)
};

}
