#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"

namespace component
{

class TempoSection : public nui::Section, juce::Timer
{
public:
    TempoSection(std::string identifier, ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor);
    ~TempoSection() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    PluginAudioProcessor& _audioProcessor;
    double _bpm = 120.0;
    nelement::Value _bpmDisplay {  "bpm-display", "BPM Display", "BPM" };
    
    void updateBPMDisplay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TempoSection)
};

}
