#pragma once
#include "Entry.h"

namespace LEO
{
    class IChannel
    {
    public:
        virtual void Submit(const Entry& e) const = 0;
        virtual void SetLoggingLevel(LogLevel level) = 0;

        virtual ~IChannel() = default;
    };

    class ConsoleChannel : public IChannel
    {
    public:
        void Submit(const Entry& e) const override;
        virtual void SetLoggingLevel(LogLevel level);

        ~ConsoleChannel() {};
    private:
        LogLevel m_level = LogLevel::INFO;
    };
}