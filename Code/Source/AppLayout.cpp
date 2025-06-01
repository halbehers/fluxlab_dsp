#include "AppLayout.h"
#include "Parameters.h"

AppLayout::AppLayout(ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor):
    nlayout::AppLayout(parameterManager, Parameters::PLUGIN_ENABLED_ID),
    _audioLevels("audio-levels", audioProcessor.rmsProcessor),
    _audioSpectrum("audio-spectrum", audioProcessor, &audioProcessor.leftChannelFifo, &audioProcessor.rightChannelFifo),
    _tempoSection("tempo-section", parameterManager),
    _crossfader(parameterManager, Parameters::CROSSFADER_VALUE_ID),
    _tabs("tabs", parameterManager),
    _fxGrid(audioProcessor, audioProcessor.audioEngine)
{
    
    getLayout().setGap(16.f);
    getLayout().setDisplayGrid(false);
    getLayout().setResizableLineConfiguration({ .displayLine = false });

    getLayout().setMargin(24.f, 12.f, 24.f, 12.f + 20.f + 16.f);
    getLayout().init({ 3, 12, 40, 5 }, { 1, 16, 3, 11, 3, 17 });

    getLayout().addComponent(_title, 0, 0, 6, 1);
    getLayout().addComponent(_audioLevels, 1, 0, 1, 1);
    getLayout().addComponent(_audioSpectrum, 1, 1, 5, 1);
    getLayout().addComponent(_tempoSection, 3, 0, 2, 1);
    getLayout().addComponent(_crossfader, 3, 3, 1, 1);
    //getLayout().addComponent(_tabs, 3, 5, 1, 1);
    getLayout().addComponent(_fxGrid, 2, 0, 6, 1);

    displayTooltip();
}

void AppLayout::bypassComponents(bool isBypassed)
{
    _fxGrid.setBypass(isBypassed);
}

