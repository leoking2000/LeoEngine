#pragma once
#include "Level.h"

namespace LEO
{
    struct Entry
    {
        LogLevel level = LogLevel::NONE;
        std::string note = "";
        const char* sourceFile;
        const char* sourceFunctionName;
        int sourceLine;
    };
}