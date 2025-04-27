#include "Component/FXGrid.h"
#include "Component/Section/SectionIDs.h"

namespace component
{

FXGrid::FXGrid(ndsp::ParameterManager& parameterManager):
    nui::Section("fx-grid", parameterManager),
    _layout(*this),
    _reverb(parameterManager),
    _delay("delay", parameterManager),
    _flanger("flanger", parameterManager),
    _chorus("chorus",parameterManager),
    _gate("gate", parameterManager),
    _compressor("compressor", parameterManager)
{
    _layout.setGap(16.f);
    _layout.setDisplayGrid(false);
    _layout.setResizableLineConfiguration({ .displayLine = true, .displayHandle = false });
    _layout.setMovableConfiguration({ .splitHandles = true, .movableZoneHeight = 30.f });
    
    _layout.setMargin(12.f, 0.f, 12.f, 1.f);
    _layout.init({ 1, 1 }, { 1, 1, 1 });
    
    _layout.addComponent("reverb", _reverb, 0, 0, 1, 1);
    _layout.addComponent("delay", _delay, 0, 1, 1, 1);
    _layout.addComponent("flanger", _flanger, 0, 2, 1, 1);
    _layout.addComponent("chorus", _chorus, 1, 0, 1, 1);
    _layout.addComponent("gate", _gate, 1, 1, 1, 1);
    _layout.addComponent("compressor", _compressor, 1, 2, 1, 1);
    
    _layout.setResizableLine({ .position = 1, .direction = nlayout::GridLayout<nui::Component>::HORIZONTAL });
    _layout.setResizableLine({ .position = 1, .direction = nlayout::GridLayout<nui::Component>::VERTICAL });
    _layout.setResizableLine({ .position = 2, .direction = nlayout::GridLayout<nui::Component>::HORIZONTAL });
    _layout.setResizableLine({ .position = 2, .direction = nlayout::GridLayout<nui::Component>::VERTICAL });
    
    _layout.setMovable("reverb", true);
    _layout.setMovable("delay", true);
    _layout.setMovable("flanger", true);
    _layout.setMovable("chorus", true);
    _layout.setMovable("gate", true);
    _layout.setMovable("compressor", true);
}

FXGrid::~FXGrid()
{
}

void FXGrid::paint(juce::Graphics& g)
{
    Component::paint(g);
    _layout.paint(g);
}

void FXGrid::resized()
{
    _layout.resized();
}

void FXGrid::bypassComponents(bool isBypassed)
{
    _reverb.setBypass(isBypassed);
    _delay.setBypass(isBypassed);
    _flanger.setBypass(isBypassed);
    _chorus.setBypass(isBypassed);
    _gate.setBypass(isBypassed);
    _compressor.setBypass(isBypassed);
}

}
