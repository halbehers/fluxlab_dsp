/*
  ==============================================================================

    ReverbSection.cpp
    Created: 24 Apr 2025 3:48:51pm
    Author:  Sebastien Halbeher

  ==============================================================================
*/

#include "Component/Section/Reverb.h"

namespace nierika::gui::section
{

Reverb::Reverb(ndsp::ParameterManager& parameterManager):
    Section("reverb-section", parameterManager),
    _size(parameterManager, Parameters::REVERB_SIZE_ID),
    _width(parameterManager, Parameters::REVERB_WIDTH_ID),
    _highPass(parameterManager, Parameters::REVERB_HPF_ID),
    _lowPass(parameterManager, Parameters::REVERB_LPF_ID)
{
    setSectionName("Reverb");
    
    setBypassable(true);

    registerComponent(_size);
    registerComponent(_width);
    registerComponent(_highPass);
    registerComponent(_lowPass);

    initLayout();

    getLayout().setMargin(12.f, 0.f, 12.f, 12.f);
    getLayout().setDisplayGrid(false);
}

Reverb::~Reverb()
{
}

void Reverb::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
