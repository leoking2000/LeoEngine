#include <assert.h>
#include "Log.h"

namespace LEO
{
    static IChannel* g_channel = nullptr;

    void LOGInitialization(IChannel* channel)
    {
        assert(g_channel == nullptr);
        assert(channel != nullptr);

        g_channel = channel;
    }

    void LOGTerminate()
    {
        delete g_channel;
        g_channel = nullptr;
    }

    void SetDefaultLogChannel(IChannel* channel)
    {
        assert(g_channel != nullptr);
        assert(channel != nullptr);

        delete g_channel;
        g_channel = channel;
    }

    IChannel* GetDefaultLogChannel()
    {
        assert(g_channel != nullptr);
        return g_channel;
    }
}


