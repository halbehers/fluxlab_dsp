#include "Component/Section/DJFilter.h"
#include "Parameters.h"

namespace nierika::gui::section
{

DJFilter::DJFilter(ndsp::ParameterManager& parameterManager):
    Section("dj-filter-section", parameterManager, Parameters::DJ_FILTER_ENABLED_ID),
    _value(parameterManager, Parameters::DJ_FILTER_VALUE_ID),
    _q(parameterManager, Parameters::DJ_FILTER_Q_ID)
{
    setSectionName("DJ Filter");

    getActiveLayout().init({ 1, 2 }, { 4, 9, 6, 4 });

    getActiveLayout().addComponent(_value, 0, 1, 1, 2);
    getActiveLayout().addComponent(_q, 1, 2, 1, 1);

    constexpr float baseMargin = 12.f;
    constexpr float verticalMargin = 24.f;
    setLayoutMargin(baseMargin, baseMargin - 12.f + verticalMargin, baseMargin, baseMargin + verticalMargin);
    setLayoutDisplayGrid(false);
}

void DJFilter::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
