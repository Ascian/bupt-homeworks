#pragma once
#include "ConsoleModule.h"

class ReceiverModule :
    public ConsoleModule
{
public:
    ReceiverModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

