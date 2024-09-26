#include "Level.h"

namespace LEO
{
    std::string GetLevelName(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::VERBOSE : return "Verbose";
            case LogLevel::DEBUG   : return "Debug";
            case LogLevel::INFO    : return "Info";
            case LogLevel::WARN    : return "Warning";
            case LogLevel::ERROR   : return "Error";
            case LogLevel::FATAL   : return "Fatal";
            default                : return "Unknown";
        }
    }
}