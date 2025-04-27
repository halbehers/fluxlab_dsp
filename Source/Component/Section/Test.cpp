#include <JuceHeader.h>
#include "Component/Section/Test.h"
#include "Component/Section/SectionIDs.h"

namespace component::section
{

Test::Test(std::string identifier, ndsp::ParameterManager& parameterManager):
    Section(identifier, parameterManager)
{
    setComponentID(identifier);
    setName(identifier);
    nui::TooltipManager::getInstance().registerComponent(*this, identifier);
}

Test::~Test()
{
    nui::TooltipManager::getInstance().unregisterComponent(*this);
}

void Test::paint (juce::Graphics& g)
{
    displayBackground();
    displayBorder();
    Section::paint(g);
    g.drawFittedText(getComponentID(), 0, 0, getWidth(), getHeight(), juce::Justification::centred, 1);
}

void Test::resized()
{
    Section::resized();
}

}
