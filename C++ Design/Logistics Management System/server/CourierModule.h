#pragma once
#include "ConsoleModule.h"


class CourierModule :
    public ConsoleModule
{
public:
    CourierModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

