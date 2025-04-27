#include "PluginProcessor.h"
#include "PluginEditor.h"

NoteAudioProcessorEditor::NoteAudioProcessorEditor(NoteAudioProcessor& p):
    AudioProcessorEditor(&p),
    audioProcessor(p),
    _stars(800, 600),
    _layout(p, p)
{
    addAndMakeVisible(_stars, 0);
    
    addAndMakeVisible(_layout, 10);

    setSize(1100, 758);
    setResizable(true, true);
    setResizeLimits(400, 500, 5000, 5000);
}

NoteAudioProcessorEditor::~NoteAudioProcessorEditor()
{
}

void NoteAudioProcessorEditor::setBypass(bool isBypassed)
{
    _layout.setBypass(isBypassed);
}

void NoteAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::Colour primaryColor = nui::Theme::getInstance().getColor(nui::Theme::PRIMARY).asJuce();
    juce::Colour darkShadeColor = nui::Theme::getInstance().getColor(nui::Theme::DARK_SHADE).asJuce();
    g.setGradientFill(juce::ColourGradient(primaryColor, 0.0, 0.0, darkShadeColor, 800.0, 600.0, false));
    g.fillAll();
}

void NoteAudioProcessorEditor::resized()
{
    _stars.setBounds(0, 0, getWidth(), getHeight());
    _layout.setBounds(getLocalBounds());
}
