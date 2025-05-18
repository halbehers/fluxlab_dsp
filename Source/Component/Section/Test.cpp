#include "Component/Section/Test.h"

namespace component::section
{

Test::Test(const std::string& identifier, ndsp::ParameterManager& parameterManager):
    Section(identifier, parameterManager)
{
    setComponentID(identifier);
    Component::setName(identifier);
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
