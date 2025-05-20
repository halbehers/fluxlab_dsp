#include "Component/Section/Compressor.h"
#include "Parameters.h"

namespace nierika::gui::section
{

Compressor::Compressor(ndsp::ParameterManager& parameterManager):
    Section("compressor-section", parameterManager, Parameters::COMPRESSOR_ENABLED_ID),
    _threshold(parameterManager, Parameters::COMPRESSOR_THRESHOLD_ID, "dB"),
    _ratio(parameterManager, Parameters::COMPRESSOR_RATIO_ID),
    _attack(parameterManager, Parameters::COMPRESSOR_ATTACK_ID),
    _release(parameterManager, Parameters::COMPRESSOR_RELEASE_ID),
    _dryWet(parameterManager, Parameters::COMPRESSOR_DRY_WET_ID)
{
    setSectionName("Compressor");

    registerComponent(_dryWet);

    addPanel("configuration", "Configuration");

    registerComponent(_threshold, "configuration");
    registerComponent(_ratio, "configuration");
    registerComponent(_attack, "configuration");
    registerComponent(_release, "configuration");
    
    setPanelName(MAIN_PANEL_ID, "One Knob");
    
    initLayout();

    setLayoutMargin(12.f, 0.f, 12.f, 12.f);
    setLayoutDisplayGrid(false);
}

void Compressor::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
