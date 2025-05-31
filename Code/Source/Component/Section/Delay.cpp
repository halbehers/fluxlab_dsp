#include "Component/Section/Delay.h"
#include "Parameters.h"

namespace nierika::gui::section
{

Delay::Delay(ndsp::ParameterManager& parameterManager):
    Section(Parameters::DELAY_SECTION_ID, parameterManager, Parameters::DELAY_ENABLED_ID),
    _timing(parameterManager, Parameters::DELAY_TIMING_ID),
    _feedback(parameterManager, Parameters::DELAY_FEEDBACK_ID),
    _highPass(parameterManager, Parameters::DELAY_HPF_ID),
    _lowPass(parameterManager, Parameters::DELAY_LPF_ID),
    _dryWet(parameterManager, Parameters::DELAY_DRY_WET_ID)
{
    setSectionName("Delay");

    registerComponent(_dryWet);

    addPanel("configuration", "Configuration");

    registerComponent(_timing, "configuration");
    registerComponent(_feedback, "configuration");
    registerComponent(_highPass, "configuration");
    registerComponent(_lowPass, "configuration");

    setPanelName(MAIN_PANEL_ID, "One Knob");

    initLayout();

    setLayoutMargin(12.f, 0.f, 12.f, 12.f);
    setLayoutDisplayGrid(false);
}

void Delay::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
