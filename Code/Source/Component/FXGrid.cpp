#include "Component/FXGrid.h"

namespace component
{

FXGrid::FXGrid(ndsp::ParameterManager& parameterManager, ndsp::AudioEngine& audioEngine):
    nui::Section("fx-grid", parameterManager),
    _layout(*this),
    _audioEngine(audioEngine),
    _reverb(parameterManager),
    _delay(parameterManager),
    _phaser(parameterManager),
    _chorus(parameterManager),
    _filter(parameterManager),
    _compressor(parameterManager)
{
    _layout.addListener(this);
    _layout.setGap(16.f);
    _layout.setDisplayGrid(false);
    _layout.setResizableLineConfiguration({ .displayLine = true, .displayHandle = false });
    _layout.setMovableConfiguration({ .splitHandles = true, .movableZoneHeight = 30.f });
    
    _layout.setMargin(12.f, 0.f, 12.f, 1.f);
    _layout.init({ 1, 1 }, { 1, 1, 1 });
    
    _layout.addComponent(_reverb, 0, 0, 1, 1);
    _layout.addComponent(_delay, 0, 1, 1, 1);
    _layout.addComponent(_phaser, 0, 2, 1, 1);
    _layout.addComponent(_chorus, 1, 0, 1, 1);
    _layout.addComponent(_filter, 1, 1, 1, 1);
    _layout.addComponent(_compressor, 1, 2, 1, 1);

    _layout.setMovable(_reverb.getID(), true);
    _layout.setMovable(_delay.getID(), true);
    _layout.setMovable(_phaser.getID(), true);
    _layout.setMovable(_chorus.getID(), true);
    _layout.setMovable(_filter.getID(), true);
    _layout.setMovable(_compressor.getID(), true);
}

void FXGrid::paint(juce::Graphics& g)
{
    nui::Section::paint(g);
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
    _phaser.setBypass(isBypassed);
    _chorus.setBypass(isBypassed);
    _filter.setBypass(isBypassed);
    _compressor.setBypass(isBypassed);
}

void FXGrid::onItemSwaped(const std::string& firstSwapedItemID, const std::string& secondSwapedItemID)
{
    _audioEngine.swapProcesses(firstSwapedItemID, secondSwapedItemID);
}

}
