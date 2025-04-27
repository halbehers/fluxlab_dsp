#include <JuceHeader.h>
#include "Component/Section/BPMDisplay.h"

namespace component::section
{

BPMDisplay::BPMDisplay(std::string identifier, ndsp::ParameterManager& parameterManager, NoteAudioProcessor& audioProcessor):
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
