#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"

namespace component::section
{

class Test : public nui::Section
{
public:
    Test(std::string identifier, ndsp::ParameterManager& parameterManager);
    ~Test() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Test)
};

}
