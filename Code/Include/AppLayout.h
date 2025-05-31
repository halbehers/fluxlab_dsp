#pragma once

#include <nierika_dsp/nierika_dsp.h>

#include "PluginProcessor.h"
#include "Component/Section/Test.h"
#include "Component/Section/Tempo.h"
#include "Component/FXGrid.h"

class AppLayout : public nlayout::AppLayout
{
public:
    AppLayout(ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor);
    ~AppLayout() override = default;
    
    void bypassComponents(bool isBypassed) override;

private:
    nelement::Title _title { "Flux Lab" };
    nelement::Levels _audioLevels;
    nelement::SpectrumAnalyzer _audioSpectrum;
    nierika::gui::section::Tempo _tempoSection;
    nelement::Crossfader _crossfader;
    component::section::Test _tabs;
    component::FXGrid _fxGrid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLayout)
};
