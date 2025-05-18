#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

PluginAudioProcessor::PluginAudioProcessor()
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
    _audioEngine.setEnabled(Parameters::PLUGIN_ENABLED_DEFAULT);
    _audioEngine.addProcess("reverb", _reverbProcess);
    
    _reverbProcess.setHPF(Parameters::REVERB_HPF_DEFAULT);
    _reverbProcess.setLPF(Parameters::REVERB_LPF_DEFAULT);
}

PluginAudioProcessor::~PluginAudioProcessor()
{
    clearParameters();
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::getParameterLayout()
{
    // General.
    registerParameter
    (
        Parameters::PLUGIN_ENABLED_ID,
        "Plugin Enabled",
        Parameters::PLUGIN_ENABLED_DEFAULT,
        [this](bool value) { _audioEngine.setEnabled(value); },
        "Bypass the whole plugin."
     );

    // Reverb.
    registerParameter
    (
        Parameters::REVERB_ENABLED_ID,
        "Reverb Enabled",
        Parameters::REVERB_ENABLED_DEFAULT,
        [this](bool value) {
            _reverbProcess.setEnabled(value);
        },
        "Bypass the reverb."
    );
    registerParameter
    (
        Parameters::REVERB_SIZE_ID,
        "Reverb Size",
        "Size",
        Parameters::REVERB_SIZE_DEFAULT,
        0.f,
        1.f,
        [this](float value) {
            _reverbProcess.setSize(value);
        },
        "Reverb's room size."
    );
    registerParameter
    (
        Parameters::REVERB_WIDTH_ID,
        "Reverb Width",
        "Width",
        Parameters::REVERB_WIDTH_DEFAULT,
        0.f,
        1.f,
        [this](float value) {
            _reverbProcess.setWidth(value);
        },
        "Reverb's width."
    );
    registerParameter
    (
        Parameters::REVERB_HPF_ID,
        "Reverb HPF",
        "High Pass",
        Parameters::REVERB_HPF_DEFAULT,
        20.f,
        20000.f,
        [this](float value) { _reverbProcess.setHPF(value); },
        "Reverb's high pass filter."
    );
    registerParameter
    (
        Parameters::REVERB_LPF_ID,
        "Reverb LPF",
        "Low Pass",
        Parameters::REVERB_LPF_DEFAULT,
        20.f,
        20000.f,
        [this](float value) { _reverbProcess.setLPF(value); },
        "Reverb's low pass filter."
    );
    registerParameter
    (
        Parameters::REVERB_DRY_WET_ID,
        "Reverb Dry/Wet",
        "Dry/Wet",
        Parameters::REVERB_DRY_WET_DEFAULT,
        20.f,
        20000.f,
        [this](float value) {},
        "Reverb's dry / wet mix."
    );

    return buildParameterLayout();
}

const juce::String PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String PluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void PluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    // Beat Tracking.
    _beatTracker.prepare(sampleRate);

    // Audio Visualization.
    rmsProcessor.prepare(sampleRate, 0.5f);
    leftChannelFifo.prepare(samplesPerBlock);
    rightChannelFifo.prepare(samplesPerBlock);

    // Audio Engine.
    _audioEngine.prepare(spec);
}

void PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    _beatTracker.processAudioBlock(readPtr, numSamples);

    leftChannelFifo.update(buffer);
    rightChannelFifo.update(buffer);

    rmsProcessor.process(buffer);
    
    juce::dsp::AudioBlock<float> block(buffer);

    _audioEngine.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
    return new PluginAudioProcessorEditor(*this);
}

//==============================================================================
void PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    ndsp::ParameterManager::getStateInformation(destData);
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ndsp::ParameterManager::setStateInformation(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}
