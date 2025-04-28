#pragma once

#include <JuceHeader.h>

nutils::Logger& nutils::AppLogger::get()
{
    return nutils::Logger::getInstance("Nierika");
}
