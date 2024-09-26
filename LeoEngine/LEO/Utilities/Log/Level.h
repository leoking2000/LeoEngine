#pragma once
#include <string>

namespace LEO
{
    enum LogLevel
    {
        NONE    = 0,
        FATAL   = 1,
        ERROR   = 2,
        WARN    = 3,
        INFO    = 4,
        DEBUG   = 5,
        VERBOSE = 6
    };

    std::string GetLevelName(LogLevel level);
}