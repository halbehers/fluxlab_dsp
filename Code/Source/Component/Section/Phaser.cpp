#include "Component/Section/Phaser.h"
#include "Parameters.h"

namespace nierika::gui::section
{

Phaser::Phaser(ndsp::ParameterManager& parameterManager):
    Section(Parameters::PHASER_SECTION_ID, parameterManager, Parameters::PHASER_ENABLED_ID),
    _rate(parameterManager, Parameters::PHASER_RATE_ID),
    _depth(parameterManager, Parameters::PHASER_DEPTH_ID),
    _highPass(parameterManager, Parameters::PHASER_HPF_ID),
    _lowPass(parameterManager, Parameters::PHASER_LPF_ID),
    _dryWet(parameterManager, Parameters::PHASER_DRY_WET_ID)
{
    setSectionName("Phaser");

    registerComponent(_dryWet);

    addPanel("configuration", "Configuration");

    registerComponent(_rate, "configuration");
    registerComponent(_depth, "configuration");
    registerComponent(_highPass, "configuration");
    registerComponent(_lowPass, "configuration");
    
    setPanelName(MAIN_PANEL_ID, "One Knob");
    
    initLayout();

    setLayoutMargin(12.f, 0.f, 12.f, 12.f);
    setLayoutDisplayGrid(false);
}

void Phaser::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
