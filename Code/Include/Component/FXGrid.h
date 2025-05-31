#pragma once

#include <nierika_dsp/nierika_dsp.h>

#include "../PluginProcessor.h"
#include "Component/Section/Reverb.h"
#include "Component/Section/Delay.h"
#include "Component/Section/Phaser.h"
#include "Component/Section/Chorus.h"
#include "Component/Section/DJFilter.h"
#include "Component/Section/Compressor.h"

namespace component
{

class FXGrid final : public nui::Section, public nlayout::GridLayout<nui::Component>::Listener
{
public:
    explicit FXGrid(ndsp::ParameterManager& parameterManager, ndsp::AudioEngine& audioEngine);
    ~FXGrid() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void bypassComponents(bool isBypassed) override;

    void onItemSwaped(const std::string& firstSwapedItemID, const std::string& secondSwapedItemID) override;
    
private:
    nlayout::GridLayout<nui::Component> _layout;
    ndsp::AudioEngine& _audioEngine;

    nierika::gui::section::Reverb _reverb;
    nierika::gui::section::Delay _delay;
    nierika::gui::section::Phaser _phaser;
    nierika::gui::section::Chorus _chorus;
    nierika::gui::section::DJFilter _filter;
    nierika::gui::section::Compressor _compressor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXGrid)
};

}
