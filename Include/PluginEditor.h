#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AppLayout.h"

class PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PluginAudioProcessorEditor(PluginAudioProcessor&);
    ~PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    
    void resized() override;
    
    void setBypass(bool isBypassed);

private:
    PluginAudioProcessor& audioProcessor;

    nelement::Stars _stars;

    AppLayout _layout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginAudioProcessorEditor)
};
