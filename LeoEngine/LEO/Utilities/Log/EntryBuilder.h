#pragma once
#include "Entry.h"

namespace LEO
{
    class IChannel;

    class EntryBuilder : private Entry
    {
    public:
        EntryBuilder(const char* file, const char* functionName, int line);
        ~EntryBuilder();

        EntryBuilder& Level(LogLevel l);
        EntryBuilder& Note(std::string n);
        EntryBuilder& Chan(IChannel* c);

        EntryBuilder& Verbose(std::string n = "");
        EntryBuilder& Debug(std::string n = "");
        EntryBuilder& Info(std::string n = "");
        EntryBuilder& Warn(std::string n = "");
        EntryBuilder& Error(std::string n = "");
        EntryBuilder& Fatal(std::string n = "");
    private:
        IChannel* m_channel = nullptr;
    };
}