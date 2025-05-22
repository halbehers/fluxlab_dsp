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

    registerComponent(_value);
    registerComponent(_q);

    initLayout();

    constexpr float baseMargin = 12.f;
    constexpr float horizontalMargin = 32.f;
    setLayoutMargin(baseMargin, baseMargin - 12.f + horizontalMargin, baseMargin, baseMargin + horizontalMargin);
    setLayoutDisplayGrid(false);
}

void DJFilter::paint(juce::Graphics& g)
{
    displayBorder();
    displayBackground();
    Section::paint(g);
}

}
