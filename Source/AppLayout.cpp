#include "AppLayout.h"
#include "Component/Section/SectionIDs.h"

AppLayout::AppLayout(ndsp::ParameterManager& parameterManager, NoteAudioProcessor& audioProcessor):
    nlayout::AppLayout(parameterManager),
    _audioLevels("audio-levels", audioProcessor.rmsProcessor),
    _audioSpectrum("audio-spectrum", audioProcessor, &audioProcessor.leftChannelFifo, &audioProcessor.rightChannelFifo),
    _tempoSection("tempo-section", parameterManager, audioProcessor),
    _tabs("tabs", parameterManager),
    _fxGrid(audioProcessor),
    _footer("footer", parameterManager)
{
    setBypassable(true);
    
    getLayout().setGap(16.f);
    getLayout().setDisplayGrid(false);
    getLayout().setResizableLineConfiguration({ .displayLine = false });

    getLayout().setMargin(24.f, 12.f, 24.f, 12.f + 20.f + 16.f);
    getLayout().init({ 1, 3, 2, 10, 1 }, { 1, 30, 22 });

    getLayout().addComponent("title", _title, 0, 0, 3, 1);
    getLayout().addComponent("audio-levels", _audioLevels, 1, 0, 1, 1);
    getLayout().addComponent("audio-spectrum", _audioSpectrum, 1, 1, 2, 1);
    getLayout().addComponent("tempo-section", _tempoSection, 2, 0, 2, 1);
    getLayout().addComponent("tabs", _tabs, 2, 2, 1, 1);
    getLayout().addComponent("fx-grid", _fxGrid, 3, 0, 3, 1);
    getLayout().addComponent("footer", _footer, 4, 0, 3, 1);
    
    displayTooltip();
}

AppLayout::~AppLayout()
{
}

void AppLayout::bypassComponents(bool isBypassed)
{
    _fxGrid.setBypass(isBypassed);
}
