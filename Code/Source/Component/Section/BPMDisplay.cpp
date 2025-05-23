#include "Component/Section/BPMDisplay.h"

namespace component::section
{

BPMDisplay::BPMDisplay(const std::string& identifier, ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor):
    Section(identifier, parameterManager),
    _audioProcessor(audioProcessor)
{
    startTimer(60);
}

BPMDisplay::~BPMDisplay()
{
    stopTimer();
}

void BPMDisplay::timerCallback()
{
    _bpm = _audioProcessor.getBPM();
    repaint();
}

void BPMDisplay::paint (juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    
    Section::paint(g);
    
    std::string bpm = nui::Formatter::formatBPM(_bpm);
    
    g.drawFittedText(bpm, 0, 0, getWidth(), getHeight(), juce::Justification::centred, 1);
}

void BPMDisplay::resized()
{
    Section::resized();
}

}
