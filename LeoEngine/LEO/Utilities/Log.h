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

#define _leolog LEO::EntryBuilder{__FILE__, __FUNCTION__, __LINE__}.Chan(LEO::GetDefaultLogChannel())

#if !PRODUCTION_BUILD
    #define LOGFATAL(msg, ...) _leolog.Fatal(std::format(msg, __VA_ARGS__))
    #define LOGERROR(msg, ...) _leolog.Error(std::format(msg, __VA_ARGS__))
    #define LOGWARN(msg, ...) _leolog.Warn(std::format(msg, __VA_ARGS__))
    #define LOGINFO(msg, ...) _leolog.Info(std::format(msg, __VA_ARGS__))
#else
    #define LOGFATAL(msg, ...)
    #define LOGERROR(msg, ...)
    #define LOGWARN(msg, ...)
    #define LOGINFO(msg, ...)
#endif

#if LEODEBUG
    #define LOGDEBUG(msg, ...) _leolog.Debug(std::format(msg, __VA_ARGS__))
    #define LOGVERBOSE(msg, ...) _leolog.Verbose(std::format(msg, __VA_ARGS__))

#else
    #define LOGDEBUG(msg, ...)
    #define LOGVERBOSE(msg, ...)
#endif



