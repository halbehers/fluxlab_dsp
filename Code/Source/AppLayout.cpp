#include "AppLayout.h"
#include "Parameters.h"

AppLayout::AppLayout(ndsp::ParameterManager& parameterManager, PluginAudioProcessor& audioProcessor):
    nlayout::AppLayout(parameterManager, Parameters::PLUGIN_ENABLED_ID),
    _audioLevels("audio-levels", audioProcessor.rmsProcessor),
    _audioSpectrum("audio-spectrum", audioProcessor, &audioProcessor.leftChannelFifo, &audioProcessor.rightChannelFifo),
    _tempoSection("tempo-section", parameterManager),
    _tabs("tabs", parameterManager),
    _fxGrid(audioProcessor)
{
    
    getLayout().setGap(16.f);
    getLayout().setDisplayGrid(false);
    getLayout().setResizableLineConfiguration({ .displayLine = false });

    getLayout().setMargin(24.f, 12.f, 24.f, 12.f + 20.f + 16.f);
    getLayout().init({ 3, 12, 5, 40 }, { 1, 10, 40 });

    getLayout().addComponent("title", _title, 0, 0, 3, 1);
    getLayout().addComponent("audio-levels", _audioLevels, 1, 0, 1, 1);
    getLayout().addComponent("audio-spectrum", _audioSpectrum, 1, 1, 2, 1);
    getLayout().addComponent("tempo-section", _tempoSection, 2, 0, 2, 1);
    getLayout().addComponent("tabs", _tabs, 2, 2, 1, 1);
    getLayout().addComponent("fx-grid", _fxGrid, 3, 0, 3, 1);
    
    displayTooltip();
}

void AppLayout::bypassComponents(bool isBypassed)
{
    _fxGrid.setBypass(isBypassed);
}
