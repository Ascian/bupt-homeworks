#pragma once
#include "ConsoleModule.h"


class SenderModule :
    public ConsoleModule
{
public:
    SenderModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

