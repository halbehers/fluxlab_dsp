/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <nierika_dsp/nierika_dsp.h>
#include "OBTAINBeatTracker.h"
#include "AudioEngine.h"
#include "ReverbProcess.h"
#include "ChorusProcess.h"
#include "CompressorProcess.h"
#include "PhaserProcess.h"

using BlockType = juce::AudioBuffer<float>;

//==============================================================================
/**
*/
class PluginAudioProcessor : public juce::AudioProcessor, public ndsp::ParameterManager
{
public:
    //==============================================================================
    PluginAudioProcessor();
    ~PluginAudioProcessor() override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    long long getLastBeatSample() const { return _beatTracker.getLastBeatSample(); }
    double getBPM() const { return _beatTracker.getBPM(); }

    ndsp::SingleChannelSampleFIFO<BlockType> leftChannelFifo { ndsp::Channel::LEFT };
    ndsp::SingleChannelSampleFIFO<BlockType> rightChannelFifo { ndsp::Channel::RIGHT };
    ndsp::RMSProcessor rmsProcessor;
    ndsp::ReverbProcess reverbProcess;
    ndsp::PhaserProcess phaserProcess;
    ndsp::ChorusProcess chorusProcess;
    ndsp::CompressorProcess compressorProcess;
    ndsp::AudioEngine audioEngine;

private:
    dsp::OBTAINBeatTracker _beatTracker;
    double _currentSampleRate = 44100.0;
    int _blockSize = 512;

    juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
