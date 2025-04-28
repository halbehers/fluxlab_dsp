#include <JuceHeader.h>
#include "Component/TempoSection.h"

namespace component
{

TempoSection::TempoSection(std::string identifier, ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor):
    Section(identifier, parameterManager),
    _audioProcessor(audioProcessor)
{
    startTimer(60);
    addAndMakeVisible(_bpmDisplay);
    _bpmDisplay.setMargin(12.f, 8.f);
    _bpmDisplay.setPadding(8.f, 8.f);
}

TempoSection::~TempoSection()
{
    stopTimer();
}

void TempoSection::timerCallback()
{
    _bpm = _audioProcessor.getBPM();
    updateBPMDisplay();
    repaint();
}

void TempoSection::updateBPMDisplay()
{
    _bpmDisplay.setValue(nui::Formatter::formatDouble(_bpm));
}

void TempoSection::paint(juce::Graphics& g)
{
    displayBackground();
    displayBorder();
    Section::paint(g);
}

void TempoSection::resized()
{
    Section::resized();
    _bpmDisplay.setBounds(getLocalBounds());
}

}
