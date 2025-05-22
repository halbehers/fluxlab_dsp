#pragma once

#include <nierika_dsp/nierika_dsp.h>

#include "../PluginProcessor.h"
#include "Component/Section/Test.h"
#include "Component/Section/Reverb.h"
#include "Component/Section/Phaser.h"
#include "Component/Section/Chorus.h"
#include "Component/Section/DJFilter.h"
#include "Component/Section/Compressor.h"

namespace component
{

class FXGrid final : public nui::Section
{
public:
    explicit FXGrid(ndsp::ParameterManager& parameterManager);
    ~FXGrid() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void bypassComponents(bool isBypassed) override;
    
private:
    nlayout::GridLayout<Component> _layout;

    nierika::gui::section::Reverb _reverb;
    section::Test _delay;
    nierika::gui::section::Phaser _phaser;
    nierika::gui::section::Chorus _chorus;
    nierika::gui::section::DJFilter _filter;
    nierika::gui::section::Compressor _compressor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXGrid)
};

}
