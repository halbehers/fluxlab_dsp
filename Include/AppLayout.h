#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "Component/Section/BPMDisplay.h"
#include "Component/Section/Test.h"
#include "Component/TempoSection.h"
#include "Component/FXGrid.h"

class AppLayout : public nlayout::AppLayout
{
public:
    AppLayout(ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor);
    ~AppLayout() override;
    
    void bypassComponents(bool isBypassed) override;

private:
    nelement::Title _title { "Flux Lab" };
    nelement::Levels _audioLevels;
    nelement::SpectrumAnalyzer _audioSpectrum;
    component::TempoSection _tempoSection;
    component::section::Test _tabs;
    component::FXGrid _fxGrid;
    component::section::Test _footer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLayout)
};
