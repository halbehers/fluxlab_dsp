#include "Component/Section/Tempo.h"

#include "Parameters.h"

namespace nierika::gui::section
{

Tempo::Tempo(const std::string& identifier, ndsp::ParameterManager& parameterManager):
    Section(identifier, parameterManager),
    _selector(parameterManager, Parameters::TEMPO_VALUE_ID)
{
    registerComponent(_selector);

    initLayout();
}

void Tempo::paint (juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    
    Section::paint(g);
}

void Tempo::resized()
{
    Section::resized();
}

}
