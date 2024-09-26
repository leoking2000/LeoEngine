#include "Log/EntryBuilder.h"
#include "Log/Channel.h"
#include "LeoAssert.h"

namespace LEO
{
    void LOGInitialization(IChannel* channel);
    void LOGTerminate();

    void      SetDefaultLogChannel(IChannel* channel);
    IChannel* GetDefaultLogChannel();
}

#define LEOLOG LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Chan(LEO::GetDefaultLogChannel())

#if !PRODUCTION_BUILD
    #define LOGFATAL(msg, ...)   LEOLOG.Fatal(std::format(msg, __VA_ARGS__))
    #define LOGERROR(msg, ...)   LEOLOG.Error(std::format(msg, __VA_ARGS__))
    #define LOGWARN(msg, ...)    LEOLOG.Warn(std::format(msg, __VA_ARGS__))
    #define LOGINFO(msg, ...)    LEOLOG.Info(std::format(msg, __VA_ARGS__))
    #define LOGDEBUG(msg, ...)   LEOLOG.Debug(std::format(msg, __VA_ARGS__))
    #define LOGVERBOSE(msg, ...) LEOLOG.Verbose(std::format(msg, __VA_ARGS__))    
#else
    #define LOGFATAL(msg, ...)
    #define LOGERROR(msg, ...)
    #define LOGWARN(msg, ...)
    #define LOGINFO(msg, ...)
    #define LOGDEBUG(msg, ...)
    #define LOGVERBOSE(msg, ...)
#endif





