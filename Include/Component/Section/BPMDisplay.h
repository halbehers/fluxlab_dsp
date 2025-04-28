#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"

namespace component::section
{

class BPMDisplay : public nui::Section, juce::Timer
{
public:
    BPMDisplay(std::string identifier, ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor);
    ~BPMDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    PluginAudioProcessor& _audioProcessor;
    double _bpm = 120.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BPMDisplay)
};

}
