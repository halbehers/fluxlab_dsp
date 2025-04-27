#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "NoteLogger.h"
#include "Component/Section/SectionIDs.h"

NoteAudioProcessor::NoteAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), ndsp::ParameterManager(dynamic_cast<juce::AudioProcessor&>(*this), [this]() { return getParameterLayout(); })
#endif
{
}

NoteAudioProcessor::~NoteAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout NoteAudioProcessor::getParameterLayout()
{
    // General.
    registerParameter(Parameters::PLUGIN_ENABLED_ID, "Plugin Enabled", Parameters::PLUGIN_ENABLED_DEFAULT, [this](bool value) { _isPluginEnabled = value; }, "Bypass the whole plugin.");

    // Reverb.
    registerParameter(Parameters::REVERB_ENABLED_ID, "Reverb Enabled", Parameters::REVERB_ENABLED_DEFAULT, [this](bool value) { _isReverbEnabled = value; }, "Bypass the reverb.");
    registerParameter(Parameters::REVERB_SIZE_ID, "Reverb Size", "Size", Parameters::REVERB_SIZE_DEFAULT, 0.f, 1.f, [this](float value) { _reverbSize = value; }, "Reverb's room size.");
    registerParameter(Parameters::REVERB_WIDTH_ID, "Reverb Width", "Width", Parameters::REVERB_WIDTH_DEFAULT, 0.f, 1.f, [this](float value) { _reverbWidth = value; }, "Reverb's width.");
    registerParameter(Parameters::REVERB_HPF_ID, "Reverb HPF", "High Pass", Parameters::REVERB_HPF_DEFAULT, 20.f, 20000.f, [this](float value) { _reverbHPF = value; }, "Reverb's high pass filter.");
    registerParameter(Parameters::REVERB_LPF_ID, "Reverb LPF", "Low Pass", Parameters::REVERB_LPF_DEFAULT, 20.f, 20000.f, [this](float value) { _reverbLPF = value; }, "Reverb's low pass filter.");

    return buildParameterLayout();
}

const juce::String NoteAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoteAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoteAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoteAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoteAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoteAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoteAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoteAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String NoteAudioProcessor::getProgramName(int index)
{
    return {};
}

void NoteAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void NoteAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    _beatTracker.prepare(sampleRate);
    
    rmsProcessor.prepare(sampleRate, 0.5f);
    
    leftChannelFifo.prepare(samplesPerBlock);
    rightChannelFifo.prepare(samplesPerBlock);
}

void NoteAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoteAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NoteAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Zero any outputs that are beyond the input channel count
    for (int ch = totalNumInputChannels; ch < totalNumOutputChannels; ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());

    // For beat tracking, let's just read from the 1st channel
    auto* readPtr = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();

    // If you want to pass audio through unchanged, you can do:
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        auto* in  = buffer.getReadPointer(ch);
        auto* out = buffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i)
            out[i] = in[i]; // pass-through
    }
    // or do some DSP...

    _beatTracker.processAudioBlock(readPtr, numSamples);

    leftChannelFifo.update(buffer);
    rightChannelFifo.update(buffer);

    rmsProcessor.process(buffer);
}

//==============================================================================
bool NoteAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoteAudioProcessor::createEditor()
{
    return new NoteAudioProcessorEditor(*this);
}

//==============================================================================
void NoteAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    ndsp::ParameterManager::getStateInformation(destData);
}

void NoteAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ndsp::ParameterManager::setStateInformation(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoteAudioProcessor();
}
