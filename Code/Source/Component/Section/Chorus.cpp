#include "Component/Section/Chorus.h"
#include "Parameters.h"

namespace nierika::gui::section
{

Chorus::Chorus(ndsp::ParameterManager& parameterManager):
    Section("chorus-section", parameterManager, Parameters::CHORUS_ENABLED_ID),
    _rate(parameterManager, Parameters::CHORUS_RATE_ID),
    _depth(parameterManager, Parameters::CHORUS_DEPTH_ID),
    _highPass(parameterManager, Parameters::CHORUS_HPF_ID),
    _lowPass(parameterManager, Parameters::CHORUS_LPF_ID),
    _dryWet(parameterManager, Parameters::CHORUS_DRY_WET_ID)
{
    setSectionName("Chorus");

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

void Chorus::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
