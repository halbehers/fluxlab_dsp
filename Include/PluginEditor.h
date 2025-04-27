#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AppLayout.h"

class NoteAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NoteAudioProcessorEditor(NoteAudioProcessor&);
    ~NoteAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    
    void resized() override;
    
    void setBypass(bool isBypassed);

private:
    NoteAudioProcessor& audioProcessor;

    nelement::Stars _stars;

    AppLayout _layout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteAudioProcessorEditor)
};
