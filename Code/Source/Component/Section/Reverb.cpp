#include "Component/Section/Reverb.h"
#include "Parameters.h"

namespace nierika::gui::section
{

Reverb::Reverb(ndsp::ParameterManager& parameterManager):
    Section("reverb-section", parameterManager, Parameters::REVERB_ENABLED_ID),
    _size(parameterManager, Parameters::REVERB_SIZE_ID),
    _width(parameterManager, Parameters::REVERB_WIDTH_ID),
    _highPass(parameterManager, Parameters::REVERB_HPF_ID),
    _lowPass(parameterManager, Parameters::REVERB_LPF_ID),
    _dryWet(parameterManager, Parameters::REVERB_DRY_WET_ID)
{
    setSectionName("Reverb");

    registerComponent(_dryWet);

    addPanel("configuration", "Configuration");

    registerComponent(_size, "configuration");
    registerComponent(_width, "configuration");
    registerComponent(_highPass, "configuration");
    registerComponent(_lowPass, "configuration");
    
    setPanelName(MAIN_PANEL_ID, "One Knob");
    
    initLayout();

    setLayoutMargin(12.f, 0.f, 12.f, 12.f);
    setLayoutDisplayGrid(false);
}

void Reverb::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
