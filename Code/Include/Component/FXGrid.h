#pragma once

#include <nierika_dsp/nierika_dsp.h>

#include "../PluginProcessor.h"
#include "Component/Section/Test.h"
#include "Component/Section/Reverb.h"

namespace component
{

class FXGrid : public nui::Section
{
public:
    explicit FXGrid(ndsp::ParameterManager& parameterManager);
    ~FXGrid() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void bypassComponents(bool isBypassed) override;
    
private:
    nlayout::GridLayout<nui::Component> _layout;

    nierika::gui::section::Reverb _reverb;
    component::section::Test _delay;
    component::section::Test _flanger;
    component::section::Test _chorus;
    component::section::Test _gate;
    component::section::Test _compressor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXGrid)
};

}
