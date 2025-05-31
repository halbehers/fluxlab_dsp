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
    audioEngine.setEnabled(Parameters::PLUGIN_ENABLED_DEFAULT);
    audioEngine.addProcess(Parameters::REVERB_SECTION_ID, reverbProcess);
    audioEngine.addProcess(Parameters::DELAY_SECTION_ID, delayProcess);
    audioEngine.addProcess(Parameters::PHASER_SECTION_ID, phaserProcess);
    audioEngine.addProcess(Parameters::CHORUS_SECTION_ID, chorusProcess);
    audioEngine.addProcess(Parameters::DJ_FILTER_SECTION_ID, djFilterProcess);
    audioEngine.addProcess(Parameters::COMPRESSOR_SECTION_ID, compressorProcess);

    reverbProcess.setEnabled(Parameters::REVERB_ENABLED_DEFAULT);
    reverbProcess.setSize(Parameters::REVERB_SIZE_DEFAULT);
    reverbProcess.setWidth(Parameters::REVERB_WIDTH_DEFAULT);
    reverbProcess.setHPF(Parameters::REVERB_HPF_DEFAULT);
    reverbProcess.setLPF(Parameters::REVERB_LPF_DEFAULT);
    reverbProcess.setDryWet(Parameters::REVERB_DRY_WET_DEFAULT);

    delayProcess.setEnabled(Parameters::DELAY_ENABLED_DEFAULT);
    delayProcess.setTimeInMs(Parameters::DELAY_TIME_MS_DEFAULT);
    delayProcess.setFeedback(Parameters::DELAY_FEEDBACK_DEFAULT);
    delayProcess.setHPF(Parameters::DELAY_HPF_DEFAULT);
    delayProcess.setLPF(Parameters::DELAY_LPF_DEFAULT);
    delayProcess.setDryWet(Parameters::DELAY_DRY_WET_DEFAULT);

    phaserProcess.setEnabled(Parameters::PHASER_ENABLED_DEFAULT);
    phaserProcess.setDepth(Parameters::PHASER_DEPTH_DEFAULT);
    phaserProcess.setRate(Parameters::PHASER_RATE_DEFAULT);
    phaserProcess.setHPF(Parameters::PHASER_HPF_DEFAULT);
    phaserProcess.setLPF(Parameters::PHASER_LPF_DEFAULT);
    phaserProcess.setDryWet(Parameters::PHASER_DRY_WET_DEFAULT);

    chorusProcess.setEnabled(Parameters::CHORUS_ENABLED_DEFAULT);
    chorusProcess.setDepth(Parameters::CHORUS_DEPTH_DEFAULT);
    chorusProcess.setRate(Parameters::CHORUS_RATE_DEFAULT);
    chorusProcess.setHPF(Parameters::CHORUS_HPF_DEFAULT);
    chorusProcess.setLPF(Parameters::CHORUS_LPF_DEFAULT);
    chorusProcess.setDryWet(Parameters::CHORUS_DRY_WET_DEFAULT);

    djFilterProcess.setEnabled(Parameters::DJ_FILTER_ENABLED_DEFAULT);
    djFilterProcess.setValue(Parameters::DJ_FILTER_VALUE_DEFAULT);
    djFilterProcess.setQ(Parameters::DJ_FILTER_Q_DEFAULT);

    compressorProcess.setEnabled(Parameters::COMPRESSOR_ENABLED_DEFAULT);
    compressorProcess.setThreshold(Parameters::COMPRESSOR_THRESHOLD_DEFAULT);
    compressorProcess.setRatio(Parameters::COMPRESSOR_RATIO_DEFAULT);
    compressorProcess.setAttack(Parameters::COMPRESSOR_ATTACK_DEFAULT);
    compressorProcess.setRelease(Parameters::COMPRESSOR_RELEASE_DEFAULT);
    compressorProcess.setDryWet(Parameters::COMPRESSOR_DRY_WET_DEFAULT);
}

PluginAudioProcessor::~PluginAudioProcessor()
{
    clearParameters();
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::getParameterLayout()
{
    Parameters::registerAllSections(this);

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
    (void) index;
}

const juce::String PluginAudioProcessor::getProgramName(int index)
{
    (void) index;

    return {};
}

void PluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    (void) index;
    (void) newName;
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getMainBusNumOutputChannels());

    // Beat Tracking.
    _beatTracker.prepare(sampleRate);

    // Audio Visualization.
    rmsProcessor.prepare(sampleRate, 0.5f);
    leftChannelFifo.prepare(samplesPerBlock);
    rightChannelFifo.prepare(samplesPerBlock);

    // Audio Engine.
    audioEngine.prepare(spec);
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
    (void) midiMessages;

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

    juce::dsp::AudioBlock<float> block(buffer);
    audioEngine.process(juce::dsp::ProcessContextReplacing<float>(block));

    leftChannelFifo.update(buffer);
    rightChannelFifo.update(buffer);

    rmsProcessor.process(buffer);
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
