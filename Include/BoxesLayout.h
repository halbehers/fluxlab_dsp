#pragma once

#include <JuceHeader.h>

#include "GridLayout.h"

namespace gui::layout {

class BoxesLayout : public GridLayout
{
    BoxesLayout(juce::Component& component, const int nbOfRows, const int nbOfColumns);
    ~BoxesLayout();
};

}
