#pragma once

#include <JuceHeader.h>

struct NoteLogger: public nutils::Logger, public nutils::Singleton<NoteLogger>
{
    std::string getCompanyName() override
    {
        return ProjectInfo::companyName;
    }
    
    ~NoteLogger() override {}
};

nutils::Logger& nutils::AppLogger::get()
{
    return NoteLogger::getInstance();
}
