/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "OBTAINBeatTracker.h"

using BlockType = juce::AudioBuffer<float>;

//==============================================================================
/**
*/
class NoteAudioProcessor : public juce::AudioProcessor, public ndsp::ParameterManager
{
public:
    //==============================================================================
    NoteAudioProcessor();
    ~NoteAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (BlockType&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    long long getLastBeatSample() const { return _beatTracker.getLastBeatSample(); }
    double getBPM() const { return _beatTracker.getBPM(); }

    nierika::dsp::SingleChannelSampleFIFO<BlockType> leftChannelFifo { nierika::dsp::Channel::LEFT };
    nierika::dsp::SingleChannelSampleFIFO<BlockType> rightChannelFifo { nierika::dsp::Channel::RIGHT };
    ndsp::RMSProcessor rmsProcessor;

private:
    dsp::OBTAINBeatTracker _beatTracker;
    double _currentSampleRate = 44100.0;
    int _blockSize = 512;
    
    bool _isPluginEnabled = Parameters::PLUGIN_ENABLED_DEFAULT;
    bool _isReverbEnabled = Parameters::REVERB_ENABLED_DEFAULT;
    float _reverbSize = Parameters::REVERB_SIZE_DEFAULT;
    float _reverbWidth = Parameters::REVERB_WIDTH_DEFAULT;
    float _reverbHPF = Parameters::REVERB_HPF_DEFAULT;
    float _reverbLPF = Parameters::REVERB_LPF_DEFAULT;

    juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteAudioProcessor)
};
